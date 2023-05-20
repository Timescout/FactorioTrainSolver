/*
Genetic Class definition
Author: Ethan Worth, Andrew Bergman, Mason Paladino, Nozomu Ohno
11/19/22

Derived solver for GreedyEdgeList solver
O(T * r^4 * f_r^2) ~ O(T * r^2 * f^2) complexity
   T - number of Nets tracked during polishing iterations
   r - number of routes generated by network
   f - number of factories in network
   relationship between r and f depends on Network properties
    but likely linear, which would yield
    O(T * f^4)
*/

#ifndef GREEDY_EDGE_LIST_H
#define GREEDY_EDGE_LIST_H

/////////////////
// Includes
/////////////////

#include "../Solver.h"
#include <vector>

/////////////////
// Solver Class
/////////////////

class GreedyEdgeList : public Solver {
    struct FinishEdge {
        FactoryKey start, end;
        ResourceList rl;
        double prio;
    };

    struct PolishSolution {
        typedef std::map<FactoryKey, std::list<RouteKey>> SharedList;
        typedef std::vector<RouteKey> RouteKeyTrans;
        Network net;
        Cost cost;
        SharedList shared_facts;
        RouteKeyTrans shared2net, net2shared;
    };
    
    protected:
        // sorted list of all helpful/possible edges in the network
        // generated at compile time
        std::vector<FinishEdge> edge_list;
        double DIST_W, QUANT_W;
        size_t TRACK;

        std::vector<FactoryKey> fact_list;

        PairList<Network, Cost> history;
    
    public:
        GreedyEdgeList(const Network& net, const CostFunct cos, const Constraints constr,
            double d_w = 1.0, double q_w = 3.0, size_t track = 5);
        
        // manages the creation of the edge_list
        // O(log f)
        void addEdge(FinishEdge fe);
        // O(f^2 * log f)
        void generateEdgeList();
        // O(r)
        std::vector<RouteKey> getRandomRouteOrdering(const Network& net) const;
        // O(r)
        std::vector<FactoryKey> getRandomFactoryOrdering() const;
        
        const PairList<Network, Cost>& getHistory() const;

        // solver
        // O(T * r^4 * f_r^2) ~ O(T * r^2 * f^2)
        Network solve();

        // finisher
        // makes network into valid solution
        // O(f^2)
        Network finishNetwork(const Network& net) const;
        // reducers
        // reducser number of routes
        // O(r_f)
        void spliceRoutes(PolishSolution& sltn, RouteKey r1, RouteKey r2, FactoryKey link) const;
        // O(f + r + r_f)
        void randomlySpliceRoute(PolishSolution& sltn) const;
        // O(r * (f + r + r_f)) ~ O(f^2)
        PolishSolution multiSplice(const PolishSolution& net, size_t P = 2) const; 
        // polsiher
        // uses reducer/finisher functions to optimize a given network to a more optimal form
        // O(T * r^2 * (f + r + r_f)) ~ O(T * f^3)
        Network fullyPolish(const Network& net); 
};

#endif