/*
Genetic Class definitions
Author: Ethan Worth, Andrew Bergman, Mason Paladino, Nozomu Ohno
11/19/22

Definitions for the BruteForce class.
*/

/////////////////
// Includes
/////////////////
#include "../../../headers/solutions/solvers/Genetic.h"
#include <list>
#include <assert.h>
#include <algorithm>
#include <random>
#include <list>
#include <functional>
#include <numeric>

/////////////////
// Functions
/////////////////
// constructor
Genetic::Genetic(const Network& net, const CostFunct cos, const Constraints constr, size_t iters) 
    : Solver(net, cos, constr), num_iters(iters){
    // generate list used for finishing
    generateEdgeList();
    //if(!constraints(network))
    //    network = finishNetwork(network);
}

void Genetic::addEdge(FinishEdge fe) {
    auto comp = [](const FinishEdge& a, const FinishEdge& b){ 
        return a.prio > b.prio; 
    };
    auto it = std::lower_bound(edge_list.begin(), edge_list.end(), fe, comp);
    edge_list.insert(it, fe);
}

void Genetic::generateEdgeList(double DIST_W, double QUANT_W) {
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

std::vector<size_t> Genetic::getRandomEdgeOrdering() const {
    std::vector<size_t> inds(edge_list.size());
    for(size_t i = 0; i < inds.size(); i++)
        inds[i] = i;
    std::random_shuffle(inds.begin(), inds.end());
    return inds;
}

std::vector<RouteKey> Genetic::getRandomRouteOrdering(const Network& net) const {
    std::vector<RouteKey> inds(net.getNumRoutes());
    for(size_t i = 0; i < inds.size(); i++)
        inds[i] = i;
    std::random_shuffle(inds.begin(), inds.end());
    return inds;
}

const size_t POP_SIZE = 100;
Network Genetic::solve() {
    // start with naively finished network
    std::vector<Network> best_solutions(POP_SIZE);
    std::vector<Cost> best_costs(POP_SIZE);
    for(size_t i = 0; i < POP_SIZE; i++) {
        best_solutions[i] = randomlyFinishNetwork(network); //randomStartingCondition(network);
        best_costs[i] = cost(best_solutions[i]);
    }
    // sort solutions by costs
    std::vector<size_t> sort_inds(POP_SIZE);
    for(size_t i = 0; i < POP_SIZE; i++)
        sort_inds[i] = i;
    // sort indices
    std::sort(
        sort_inds.begin(), sort_inds.end(),
        [best_costs](const size_t& lhs, const size_t& rhs){ 
            return best_costs[lhs] < best_costs[rhs];
        }
    );
    // make temps
    std::vector<Network> temp_bs(best_solutions);
    std::vector<Cost> temp_cs(best_costs);
    // assign from temps
    for(size_t i = 0; i < POP_SIZE; i++) {
        best_solutions[i] = temp_bs[sort_inds[i]];
        best_costs[i] = temp_cs[sort_inds[i]];
    }

    // get worst cost
    Cost worst_best_cost = best_costs.back();

    // for # iters
    for(size_t iter = 0; iter < num_iters; iter++) {
        // convolve parents
        std::array<size_t, POP_SIZE> sol_inds;
        for(size_t i = 0; i < POP_SIZE; i++)
            sol_inds[i] = i;
        std::random_shuffle(sol_inds.begin(), sol_inds.end());

        for(size_t sol_ind_i = 0; sol_ind_i < POP_SIZE; ) {
            // pick random count
            size_t count = std::min(size_t(rand() % 4 + 2), POP_SIZE - sol_ind_i);
            // make parent's vector
            std::vector<Network> parents(count);
            for(size_t c = 0; c < count; c++)
                parents[c] = best_solutions[sol_inds[sol_ind_i+c]];
            sol_ind_i += count;
            // convolve and mutate
            Network mutated(convolveNetworks(parents));

            // track best solution
            Cost new_cost = cost(mutated);
            // check if new best
            if(new_cost < worst_best_cost) {
                // find insertion point
                size_t ins = std::distance(
                    best_costs.begin(), 
                    std::lower_bound(best_costs.begin(), best_costs.end(), new_cost)
                );
                // shift
                for(size_t move_i = POP_SIZE - 1; move_i > ins; move_i--) {
                    best_solutions[move_i] = best_solutions[move_i-1];
                    best_costs[move_i] = best_costs[move_i-1];
                }
                // insert
                best_solutions[ins] = mutated;
                best_costs[ins] = new_cost;
                worst_best_cost = best_costs.back();
            }
        }
    }
    
    return best_solutions[0];
}

// has to pass constraints
Network Genetic::finishNetwork(const Network& net) const {
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

// same as finish, but randomly orders the edges instead of using priority queue
Network Genetic::randomlyFinishNetwork(const Network& net) const {
    // check if already finished
    if(constraints(net))
        return net;
    
    // create copy
    Network nn(net);
    
    // generate random order of edges
    std::vector<size_t> inds = getRandomEdgeOrdering();

    // while not fulfilling constraints yet
    // go over edges in random order
    //  auto = std::vector<size_t>::iterator
    for(auto it = inds.begin(); !constraints(nn); it++) {
        // assert we have edges left
        assert(it != inds.end());

        // find viable resources
        //  other Routes might not allow this route to be fully executed
        ResourceList start_free = nn.getPlace(edge_list[*it].start).getUnallocated(),
                     end_free = nn.getPlace(edge_list[*it].end).getUnallocated(),
                     viable;
        Quant mag;
        for(Resource r = Resource(0); r != Resource::COUNT; r++) {
            // find min magnitude
            mag = std::min({abs(edge_list[*it].rl[r]), abs(start_free[r]), abs(end_free[r])});
            // update edge RL
            if(signof(edge_list[*it].rl[r]) == POS_S)
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
            

            // add route
            assert(nn.addRoute(edge_list[*it].start, edge_list[*it].end, viable, inverse));
        }
    }

    // once constraints satisfied, return network
    return nn;
}

Network Genetic::randomStartingCondition(const Network& net) const {
    // randomly finish
    Network nn(net);

    nn.eraseAllRoutes();

    nn = randomlyFinishNetwork(nn);
    // randomly join
    nn = multiSplice(nn);
    return nn;
}

// probably mutliple forms of mutation
Network Genetic::mutateNetwork(const Network& net) const {
    Network nn(net);

    // randomly select mutation
    size_t total_w = std::accumulate(MUTATION_W.begin(), MUTATION_W.end(), 0);
    size_t w = rand() % total_w;
    size_t m = 0;
    while(w >= MUTATION_W[m]) {
        w -= MUTATION_W[m];
        m++;
    }
    switch (m)
    {
    // reverse
    case 0:
        nn = reverseRoute(nn, rand() % nn.getNumRoutes());
        break;
    // rotate
    case 1:
        nn = rotateRoute(nn, rand() % nn.getNumRoutes());
        break;
    // randomlySplice
    case 2:
        nn = randomlySpliceRoute(nn);
        break;
    // drop
    case 3:
        nn = multiSplice(nn);
        break;
    // polish
    case 4:
        nn = dropRoute(nn, rand() % nn.getNumRoutes());
        break;
    }
    
    return nn;
}

Network Genetic::reverseRoute(const Network& net, RouteKey r) const {
    Network nn(net);
    nn.reverseRoute(r);
    return nn;
}

Network Genetic::rotateRoute(const Network& net, RouteKey r, size_t i) const {
    Network nn(net);
    nn.rotateRoute(r, i);
    return nn;
}

Network Genetic::spliceRoutes(const Network& net, RouteKey r1, RouteKey r2, FactoryKey link) const {
    // can't splice the same Route
    if(r1 == r2)
        return net;
    else if(r1 > r2)
        return spliceRoutes(net, r2, r1, link);
    Network nn(net);
    // find Factory in both routes
    int r1_i = nn.getRoute(r1).findStop(link), r2_i = nn.getRoute(r2).findStop(link);
    // check in routes
    if(r1_i == -1 || r2_i == -1)
        return net;

    // splice
    // abc(a)       // abc(a)
    // dbe(d)       // dbc(d)
    //   bed(b)     //   bcd(b)
    // abedbc(a)    // abcdbca

    // rotate r1 and r2 so that link is at start
    nn.rotateRoute(r1, r1_i);
    nn.rotateRoute(r2, r2_i);

    // check for match before
    auto it_1 = --nn.getRoute(r1).cend(),
         it_2 = --nn.getRoute(r2).cend();
    while(it_1->first == it_2->first) {
        if(it_1 == nn.getRoute(r1).cbegin() || it_2 == nn.getRoute(r2).cbegin())
            break;
        it_1--;
        it_2--;
    }
    // find dist traversed
    size_t d = std::distance(it_1, --nn.getRoute(r1).cend());
    if(d != 0) {
        // rotate to actual start of share
        nn.rotateRoute(r1, nn.getRoute(r1).size()-d);
        nn.rotateRoute(r2, nn.getRoute(r2).size()-d);
    }
    
    // find size of match
    size_t min_size = std::min(nn.getRoute(r1).size(), nn.getRoute(r2).size());
    size_t matchSize = 1;

    // check for match after
    while(matchSize < min_size) {
        if(nn.getStop(r1, matchSize) == nn.getStop(r2, matchSize))
            matchSize++;
        else
            break;
    }

    // get copies of Routes
    Route route1 = nn.getRoute(r1), route2 = nn.getRoute(r2);
    // drop old second route so we can reallocate its 

    nn.eraseRoute(r2);

    // for inds in r2 in match
    for(size_t i = 0; i < matchSize; i++) {
        // add on RLs
        nn.setStopCommand(
            r1, i, 
            nn.getStopCommand(r1, i) + route2.getResources(i)
        );
    }
    // for remaining facts in r2
    for(size_t i = matchSize; i < route2.size(); i++) {
        // insert to r1
        nn.addStop(r1, route2[i], i, route2.getResources(i));
    }

    // clean up any remaining twicers
    route1 = nn.getRoute(r1);
    bool have_prev = false;
    FactoryKey prev(0, 0);
    size_t i = 0;
    for(auto itt = route1.cbegin(); itt != route1.cend(); itt++) {
        if(have_prev && prev == itt->first) {
            nn.setStopCommand(
                r1, i, 
                nn.getStopCommand(r1, i-1) + nn.getStopCommand(r1, i)
            );
            nn.dropStop(r1, prev, i-1);
        }
        prev = itt->first;
        have_prev = true;
        i++;
    }
    
    return nn;
}

Network Genetic::randomlySpliceRoute(const Network& net) const {
    Network nn(net);
    // perform mutations
    FactoryKey link(0, 0);
    std::vector<RouteKey>::iterator r1, r2;
    bool splice_chosen = false;

    // traverse pairs of routes in random order
    std::vector<RouteKey> inds = getRandomRouteOrdering(nn);
    for(r1 = inds.begin(); !splice_chosen && r1 != (--inds.end()); r1++) {
        for(r2 = r1+1; !splice_chosen && r2 != inds.end(); r2++) {
            // find shared facts 
            std::vector<FactoryKey> st1 = nn.getRouteStops(*r1), st2 = nn.getRouteStops(*r2);
            std::random_shuffle(st1.begin(), st1.end());
            std::random_shuffle(st2.begin(), st2.end());
            // traverse pairs of facts in random order
            for(auto fk = st1.begin(); !splice_chosen && fk != st1.end(); fk++) {
                for(auto fk2 = st2.begin(); !splice_chosen && fk2 != st2.end(); fk2++) {
                    if(*fk == *fk2) {
                        link = *fk;
                        splice_chosen = true;
                    }
                }
            }
        }
    }
    // check if splice chosen
    if(splice_chosen)
        // splice
        nn = spliceRoutes(nn, *(r1-1), *(r2-1), link);
    
    return nn;
}

// performs several splices
// P parameter controls probability that it will stop splicing
//    this function is expected to reduce the Route count from R to
//     (R+1)*(P/P+1)
//    equation adapted from: https://math.stackexchange.com/a/75970
Network Genetic::multiSplice(const Network& net, const size_t P) const {
    Network nn(net);
    // randomly join (check that join is still performed)
    size_t routeCount = nn.getNumRoutes();
    while(routeCount == nn.getNumRoutes() && (rand() % routeCount >= P)) {
        nn = randomlySpliceRoute(nn);
        routeCount--;
    }
    return nn;
}

Network Genetic::dropRoute(const Network& net, RouteKey r1) const {
    Network nn(net);
    nn.eraseRoute(r1);
    return finishNetwork(nn);
}

Network Genetic::convolveNetworks(std::vector<Network> nets) const {
    // check that we have nets
    if(nets.size() == 0)
        return finishNetwork(network);

    // copy net
    Network nn(nets.front());

    // if mutli-parents, generate convolution
    if(nets.size() > 1) {
        // empty net
        nn.eraseAllRoutes();

        // while we sill have parents
        while(nets.size()) {
            // randomly pick net
            size_t i = rand() % nets.size();
            // check that net has routes
            if(nets[i].getNumRoutes() == 0) {
                nets.erase(nets.begin()+1);
                continue;
            }
            // randomly pick route
            size_t j = rand() % nets[i].getNumRoutes();
            // try to add Route
            nn.addRoute(nets[i].getRoute(j));
            // erase Route from parent
            nets[i].eraseRoute(j);
            // check if this parent is exhausted
            if(nets[i].getNumRoutes() == 0)
                nets.erase(nets.begin() + i);
        }
    }

    // run through a finisher and return
    return multiSplice(finishNetwork(nn));
}

Network Genetic::fullyPolish(const Network& net, const size_t TRACK) const {
    // start with naively finished network
    std::vector<Network> best_solutions(TRACK, net);
    std::vector<Cost> best_costs(TRACK, cost(best_solutions[0]));
    Cost worst_best_cost = best_costs.back();


    const size_t ITERS = 1.5*net.getNumRoutes();
    // for # iters
    for(size_t iter = 0; iter < ITERS; iter++) {
        for(size_t i = 0; i < TRACK; i++) {
            // mutate
            Network mutated(randomlySpliceRoute(best_solutions[i]));

            // track best solution
            Cost new_cost = cost(mutated);
            // check if new best
            if(new_cost < worst_best_cost) {
                // find insertion point
                size_t ins = std::distance(
                    best_costs.begin(), 
                    std::lower_bound(best_costs.begin(), best_costs.end(), new_cost)
                );
                // shift
                for(size_t move_i = TRACK - 1; move_i > ins; move_i--) {
                    best_solutions[move_i] = best_solutions[move_i-1];
                    best_costs[move_i] = best_costs[move_i-1];
                }
                // insert
                best_solutions[ins] = mutated;
                best_costs[ins] = new_cost;
                worst_best_cost = best_costs.back();
            }
        }
    }
    
    return best_solutions[0];
}