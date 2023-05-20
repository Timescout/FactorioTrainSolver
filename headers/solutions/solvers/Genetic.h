/*
Genetic Class definition
Author: Ethan Worth, Andrew Bergman, Mason Paladino, Nozomu Ohno
11/19/22

Derived solver for brute force
*/

#ifndef GENETIC_H
#define GENETIC_H

/////////////////
// Includes
/////////////////

#include "../Solver.h"
#include <functional>

/////////////////
// Solver Class
/////////////////

// maybe make in-between Heursitic Solver

class Genetic : public Solver {

    struct FinishEdge {
        FactoryKey start, end;
        ResourceList rl;
        double prio;
    };

    protected:
        size_t num_iters;
        // sorted list of all helpful/possible edges in the network
        // generated at compile time
        std::vector<FinishEdge> edge_list;
        static const size_t MUTATION_COUNT = 5;
        // reverse, rotate, splice, mutliSplice, drop
        const std::array<size_t, MUTATION_COUNT> MUTATION_W = {
            1, 1, 5, 1, 3
        };
    public:
        //////////////
        // construct
        //////////////
        Genetic(const Network& net, const CostFunct cos, const Constraints constr, size_t iters = 100);

        // tools to build out edgelist for completers
        void addEdge(FinishEdge fe);
        void generateEdgeList(double DIST_W = 1.0, double QUANT_W = 3.0);
        std::vector<size_t> getRandomEdgeOrdering() const;
        std::vector<RouteKey> getRandomRouteOrdering(const Network& net) const;

        // solver
        Network solve();

        //////////////
        // complete
        // TAKES: any valid net/solution
        // RETURNS: valid solution (based off input net)
        //////////////
        // complete network so that it passes constraints
        Network finishNetwork(const Network& net) const;
        // randomly complete network
        Network randomlyFinishNetwork(const Network& net) const;
        
        //////////////
        // generate
        // TAKES: any valid net/solution
        // RETURNS: valid solution (solely based off factories)
        //////////////
        // can use finish/randomly finish
        Network randomStartingCondition(const Network& net) const;

        //////////////
        // mutate
        // TAKES: valid solution
        // RETURNS: valid solution
        //////////////

        // parent function for mutations?
        Network mutateNetwork(const Network& net) const;
        
        // trivial alterations
        Network reverseRoute(const Network& net, RouteKey r) const;
        Network rotateRoute(const Network& net, RouteKey r, size_t i = 1) const;
        // reduce route count, form of polishing
        Network spliceRoutes(const Network& net, RouteKey r1, RouteKey r2, FactoryKey link) const;
        Network randomlySpliceRoute(const Network& net) const;
        Network multiSplice(const Network& net, const size_t P = 2) const; 
        // drops given route and runs finisher
        Network dropRoute(const Network& net, RouteKey r1) const;

        //////////////
        // convolve
        // TAKES: two valid solutions
        // RETURNS: valid solution (based off inputs)
        //////////////
        Network convolveNetworks(std::vector<Network> nets) const;
        

        //////////////
        // polish
        // TAKES: valid solution
        // RETURNS: valid solution (based off input)
        //////////////
        // uses reducer/finisher functions to optimize a given network to a more optimal form
        Network fullyPolish(const Network& net, const size_t TRACK = 5) const; 
};

#endif
