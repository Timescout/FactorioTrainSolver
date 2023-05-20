/*
Genetic Class definitions
Author: Ethan Worth, Andrew Bergman, Mason Paladino, Nozomu Ohno
11/19/22

Definitions for the GreedyEdgeList solver class.
*/

/////////////////
// Includes
/////////////////
#include "../../../headers/solutions/solvers/GreedyEdgeList.h"
#include <algorithm>
#include <assert.h>

/////////////////
// Functions
/////////////////

GreedyEdgeList::GreedyEdgeList(const Network& net, const CostFunct cos, const Constraints constr, double d_w, double q_w, size_t track) 
    : Solver(net, cos, constr), DIST_W(d_w), QUANT_W(q_w), TRACK(track) {
    // generate list used for finishing
    generateEdgeList();
    // generate FactoryKey list
    for(auto it = network.factCBegin(); it != network.factCEnd(); it++)
        fact_list.push_back(it->first);
}

// O(log f)
//    O(log [edges])
//    [edges] is f^2 complexity
//    O(log f^2) -> O (log f)
void GreedyEdgeList::addEdge(FinishEdge fe) {
    auto comp = [](const FinishEdge& a, const FinishEdge& b){ 
        return a.prio > b.prio; 
    };
    // sort container, so this is log n
    auto it = std::lower_bound(edge_list.begin(), edge_list.end(), fe, comp);
    edge_list.insert(it, fe);
}

// O(f^2 * log f)
//   f^2 pairs to compare
//   log f to insert edge to sorted list
void GreedyEdgeList::generateEdgeList() {
    ResourceList rla, rlb, rl_shared;

    // for every pair of factories
    // auto = std::map<FactoryKey, Factory, LocationCompare>::const_iterator
    for (auto it_a = network.factCBegin(); it_a != network.factCEnd(); it_a++) {
        rla = it_a->second.getBaseQuants();
        // nothing to be done for a Junction
        if(rla == ResourceList())
            continue;
        
        // auto = std::map<FactoryKey, Factory, LocationCompare>::const_iterator
        for (auto it_b = std::next(it_a); it_b != network.factCEnd(); it_b++) {
            rlb = it_b->second.getBaseQuants();
            // if Factories could fulfill each other's demands
            // add edge [useful RLs, Dist]
            rl_shared = ResourceList();
            Quant q = 0;
            for(Resource r = Resource(0); r != Resource::COUNT; r++) {
                if(signof(rla[r]) != signof(rlb[r]) && signof(rla[r]) != ZERO_S && signof(rlb[r]) != ZERO_S) {
                    Quant mag = std::min(abs(rla[r]), abs(rlb[r]));
                    if(signof(rla[r]) == POS_S)
                        rl_shared[r] = mag;
                    else
                        rl_shared[r] = -mag;
                    q += mag;
                }
            }
            // if this edge is useful
            if(q != 0) {
                // calculate prio
                double prio = 0;
                prio -= DIST_W*dist(it_a->second.getLoc(), it_b->second.getLoc()).toDouble();
                prio += QUANT_W*q;
                // add edge
                addEdge((FinishEdge){
                    it_a->first, 
                    it_b->first, 
                    rl_shared,
                    prio
                });
            }
        }
    }
}

// O(r)
std::vector<RouteKey> GreedyEdgeList::getRandomRouteOrdering(const Network& net) const {
    std::vector<RouteKey> inds(net.getNumRoutes());
    for(size_t i = 0; i < inds.size(); i++)
        inds[i] = i;
    std::random_shuffle(inds.begin(), inds.end());
    return inds;
}

// O(f)
std::vector<FactoryKey> GreedyEdgeList::getRandomFactoryOrdering() const {
    std::vector<FactoryKey>fs(fact_list.begin(), fact_list.end());
    std::random_shuffle(fs.begin(), fs.end());
    return fs;
}

const PairList<Network, Cost>& GreedyEdgeList::getHistory() const {
    return history;
}


// solver
// O(T * r^4 * f_r^2) ~ O(T * r^2 * f^2)
//   T := TRACK
Network GreedyEdgeList::solve() {
    return fullyPolish(finishNetwork(network));
}


// O(f^2)
// same order as edges
Network GreedyEdgeList::finishNetwork(const Network& net) const {
    // check if already finished
    if(constraints(net))
        return net;
    
    // create copy
    Network nn(net);
    
    // while not fulfilling constraints yet
    // go over edges in order of priority
    //  auto = std::vector<FinishEdge>::iterator
    for(auto it = edge_list.begin(); !constraints(nn); it++) {
        // assert we have edges left
        assert(it != edge_list.end());

        // find viable resources
        //  other Routes might not allow this route to be fully executed
        ResourceList start_free = nn.getPlace(it->start).getUnallocated(),
                     end_free = nn.getPlace(it->end).getUnallocated(),
                     viable;
        Quant mag;
        for(Resource r = Resource(0); r != Resource::COUNT; r++) {
            // find min magnitude
            mag = std::min({abs(it->rl[r]), abs(start_free[r]), abs(end_free[r])});
            // update edge RL
            if(signof(it->rl[r]) == POS_S)
                viable[r] = mag;
            else
                viable[r] = -mag;
        }
        
        // if edge still viable
        if(!(viable == ResourceList())) {
            // find inverse of edge RL
            ResourceList inverse;
            for(Resource r = Resource(0); r != Resource::COUNT; r++)
                inverse[r] = -viable[r];
            
            assert(nn.addRoute(it->start, it->end, viable, inverse));
        }
    }

    // once constraints satisfied, return network
    return nn;
}


// O(r_f + r)
void GreedyEdgeList::spliceRoutes(PolishSolution& sltn, RouteKey r1, RouteKey r2, FactoryKey link) const {
    // can't splice the same Route
    if(r1 == r2)
        return;
    else if(r1 > r2) {
        spliceRoutes(sltn, r2, r1, link);
        return;
    }
    // find Factory in both routes
    int r1_i = sltn.net.getRoute(r1).findStop(link), r2_i = sltn.net.getRoute(r2).findStop(link);
    // check in routes
    if(r1_i == -1 || r2_i == -1)
        return;

    // splice
    // abc(a)       // abc(a)
    // dbe(d)       // dbc(d)
    //   bed(b)     //   bcd(b)
    // abedbc(a)    // abcdbca

    // rotate r1 and r2 so that link is at start
    sltn.net.rotateRoute(r1, r1_i);
    sltn.net.rotateRoute(r2, r2_i);

    // check for match before
    auto it_1 = --sltn.net.getRoute(r1).cend(),
         it_2 = --sltn.net.getRoute(r2).cend();
    while(it_1->first == it_2->first) {
        if(it_1 == sltn.net.getRoute(r1).cbegin() || it_2 == sltn.net.getRoute(r2).cbegin())
            break;
        it_1--;
        it_2--;
    }
    // find dist traversed
    size_t d = std::distance(it_1, --sltn.net.getRoute(r1).cend());
    if(d != 0) {
        // rotate to actual start of share
        sltn.net.rotateRoute(r1, sltn.net.getRoute(r1).size()-d);
        sltn.net.rotateRoute(r2, sltn.net.getRoute(r2).size()-d);
    }
    
    // find size of match
    size_t min_size = std::min(sltn.net.getRoute(r1).size(), sltn.net.getRoute(r2).size());
    size_t matchSize = 1;

    // check for match after
    while(matchSize < min_size) {
        if(sltn.net.getStop(r1, matchSize) == sltn.net.getStop(r2, matchSize))
            matchSize++;
        else
            break;
    }

    // get copies of Routes
    Route route1 = sltn.net.getRoute(r1), route2 = sltn.net.getRoute(r2);

    // drop old second route so we can reallocate its 
    sltn.net.eraseRoute(r2);

    // for inds in r2 in match
    for(size_t i = 0; i < matchSize; i++) {
        // add on RLs
        sltn.net.setStopCommand(
            r1, i, 
            sltn.net.getStopCommand(r1, i) + route2.getResources(i)
        );
    }
    // for remaining facts in r2
    for(size_t i = matchSize; i < route2.size(); i++) {
        // insert to r1
        sltn.net.addStop(r1, route2[i], i, route2.getResources(i));
    }

    // clean up any remaining twicers
    route1 = sltn.net.getRoute(r1);
    bool have_prev = false;
    FactoryKey prev(0, 0);
    size_t i = 0;
    for(auto itt = route1.cbegin(); itt != route1.cend(); itt++) {
        if(have_prev && prev == itt->first) {
            sltn.net.setStopCommand(
                r1, i, 
                sltn.net.getStopCommand(r1, i-1) + sltn.net.getStopCommand(r1, i)
            );
            sltn.net.dropStop(r1, prev, i-1);
        }
        prev = itt->first;
        have_prev = true;
        i++;
    }
    

    // update SharedList
    // for fact in route2
    for(auto itt = route2.cbegin(); itt != route2.cend(); itt++) {
        // remove r2 from list of facts that share
        sltn.shared_facts[itt->first].remove(sltn.net2shared[r2]);
        // if r1 not already in here
        if(std::find(sltn.shared_facts[itt->first].begin(), sltn.shared_facts[itt->first].end(), sltn.net2shared[r1]) == sltn.shared_facts[itt->first].end()) {
            // add r1
            sltn.shared_facts[itt->first].push_back(sltn.net2shared[r1]);
        }
    }

    // update RouteKeyTrans
    // update shared2net
    sltn.shared2net[sltn.net2shared[r2]] = -1;
    for(size_t i = sltn.net2shared[r2]+1; i < sltn.shared2net.size(); i++)
        sltn.shared2net[i]--;
    // update net2shared
    sltn.net2shared.erase(sltn.net2shared.begin()+r2);
}

// O(f + r + r_f)
void GreedyEdgeList::randomlySpliceRoute(PolishSolution& sltn) const {
    // perform mutations
    FactoryKey link(0, 0);
    RouteKey r1, r2;
    bool splice_chosen = false;

    // traverse pairs of routes in random order
    std::vector<FactoryKey> f_ord = getRandomFactoryOrdering();
    std::vector<RouteKey> rk_temp;
    for(FactoryKey fk : f_ord) {
        auto search_it = sltn.shared_facts.find(fk);
        if(search_it != sltn.shared_facts.end() && search_it->second.size() >= 2) {
            link = fk;
            rk_temp = std::vector<RouteKey>(search_it->second.begin(), search_it->second.end());
            std::random_shuffle(rk_temp.begin(), rk_temp.end());
            r1 = sltn.shared2net[rk_temp.front()];
            r2 = sltn.shared2net[*(++(rk_temp.begin()))];
            splice_chosen = true;
            break;
        }
    }
    // check if splice chosen
    if(splice_chosen)
        // splice
        spliceRoutes(sltn, r1, r2, link);
}

// performs several splices
// O(r * (f + r + r_f)) ~ O(f^2)
//    random splice is f + r + r_f
//    perform random splice on order of r
// P parameter controls probability that it will stop splicing
//    this function is expected to reduce the Route count from R to
//     (R+1)*(P/P+1)
//    equation adapted from: https://math.stackexchange.com/a/75970
GreedyEdgeList::PolishSolution GreedyEdgeList::multiSplice(const PolishSolution& sltn, size_t P) const {
    PolishSolution n_sltn(sltn);
    // randomly join (check that join is still performed)
    size_t routeCount = n_sltn.net.getNumRoutes();
    // don't use P that's too big compared to Route size
    P = std::min(P, routeCount/2);
    // always splice at least once
    do {
        randomlySpliceRoute(n_sltn);
        routeCount--;
    } while(routeCount == n_sltn.net.getNumRoutes() && (rand() % routeCount >= P));
    n_sltn.cost = cost(n_sltn.net);
    return n_sltn;
}

// uses reducer/finisher functions to optimize a given network to a more optimal form
// O(T * r^2 * (f + r + r_f)) ~ O(T * f^3)
//    iterations on order of r
//    call multisplice on order of iters*track
//    multisplice on order of O(r * (f + r + r_f))
Network GreedyEdgeList::fullyPolish(const Network& net) {
    // generate intial shared list
    PolishSolution::SharedList base_shared;
    RouteKey rki = 0;
    for(auto r_it = net.routeCBegin(); r_it != net.routeCEnd(); r_it++) {
        for(auto f_it = r_it->cbegin(); f_it != r_it->cend(); f_it++) {
            // efficienct insert and increment from https://stackoverflow.com/a/101980
            //   taken from Effective STL by Scott Meyers
            auto search_it = base_shared.lower_bound(f_it->first);

            // if in container
            if(search_it != base_shared.end() && (search_it->first == f_it->first)) {
                // add to list
                search_it->second.push_back(rki);
            } else {
                // insert new list
                base_shared.insert(
                    search_it, 
                    std::make_pair(
                        f_it->first, 
                        (std::list<RouteKey>){rki}
                    )
                );
            }
        }
        rki++;
    }
    // create inital translator
    PolishSolution::RouteKeyTrans base_trans(net.getNumRoutes());
    for(size_t i = 0; i < base_trans.size(); i++)
        base_trans[i] = i;
    // start with naively finished network
    std::vector<PolishSolution> best_solutions(TRACK, (PolishSolution){
        net, 
        cost(net), 
        base_shared, 
        base_trans, 
        base_trans
    });
    Cost worst_best_cost = best_solutions.back().cost;

    size_t ITERS = net.getNumRoutes();
    // for # iters
    for(size_t iter = 0; iter < ITERS; iter++) {
        for(size_t i = 0; i < TRACK; i++) {
            // splice (multiple times)
            PolishSolution mutated(multiSplice(best_solutions[i], 4));

            // check if new best
            if(mutated.cost < worst_best_cost) {
                // find insertion point
                size_t ins = std::distance(
                    best_solutions.begin(), 
                    std::lower_bound(
                        best_solutions.begin(), 
                        best_solutions.end(), 
                        mutated, 
                        [](const PolishSolution& lhs, const PolishSolution& rhs){
                            return lhs.cost < rhs.cost;
                        }
                    )
                );
                // shift
                for(size_t move_i = TRACK - 1; move_i > ins; move_i--) {
                    best_solutions[move_i] = best_solutions[move_i-1];
                }
                // insert
                best_solutions[ins] = mutated;
                worst_best_cost = best_solutions.back().cost;
            }
        }
        // reduce iters if size of solutions are dropping
        ITERS = std::min(ITERS, best_solutions[0].net.getNumRoutes()+2);
        history.emplace_back(best_solutions[0].net, best_solutions[0].cost);
    }
    
    return best_solutions[0].net;
}