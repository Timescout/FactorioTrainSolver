/*
main file
Author: Ethan Worth, Andrew Bergman, Mason Paladino, Nozomu Ohno
11/19/22

main lives here.
*/

/////////////////
// Compile Commands
/////////////////

// definition for the compiler. If this is set includes tests
// and things that only should be in dev builds
// comment next line to control which is defined.
#define DEVELOPMENT


/////////////////
// Includes
/////////////////

#include <iostream>
#include <list>
#include <time.h>
#include "../headers/solutions/solvers/GreedyEdgeList.h"
#include "../headers/solutions/CanonicalExamples.h"

/////////////////
// main
/////////////////

int main() {
    const std::list<Network> CANON_NETS = {
        CANON_BASIC,
        CANON_DUAL_SERVE,
        CANON_DUAL_RES_PRODUCE,
        CANON_TWO_ZONES,
        CANON_TRI_CYCLE
    };
    std::list<std::pair<int,int>> m = {
        {29259, 35},
        {14173, 185},
        {27464, 355},
        {8787, 925},
        {23412, 995}
    };
    /*std::list<std::pair<int,int>> m = {
        {21442, 52}
    };*/
    Constraints CONS;
    Coord MAX_LOC_COORD = 1000, NUM_ITERS = 10;
    const int numFacts = 50;
    size_t start = time(0);
    for(auto p : m){
    //for(auto net : CANON_NETS) {
        std::cout << p.first << ", " << p.second << std::endl;
        Network net = randomNetwork(p.first, numFacts, p.second);
        //Network net = randomNetwork(p.first, p.second, p.second*10);
        GreedyEdgeList solv(net, ALL_COSTS, CONS);
        Cost init_cost = ALL_COSTS(solv.finishNetwork(solv.getNet()));
        Network solved = solv.solve();
        std::cout << CONS(solved) << ' ' << ALL_COSTS(solved) << " <= " << init_cost << std::endl;
        for(auto h : solv.getHistory()) {
            //std::cout << h.first.getNumRoutes() << " => " << h.second << std::endl;
        }
    }
    size_t end = time(0);
    std::cout << end - start << std::endl;
    // 44 on old solver
    // 2-3 on new

    return 0;
}

/*
-157, -495
*/