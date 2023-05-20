/*
GreedyEdgeList unit test
Author: Andrew Bergman
12/2/22

This file tests the Constraints object, defined in Constraints.h
*/

/////////////////
// Includes
/////////////////

#include <gtest/gtest.h>
#include "../headers/solutions/solvers/GreedyEdgeList.h"
#include "../headers/solutions/CanonicalExamples.h"

/////////////////
// tests
/////////////////

//Test good network for solution constraints
//Check Twice
namespace GreedyEdgeListTest {
    const std::list<Network> CANON_NETS = {
        CANON_BASIC,
        CANON_DUAL_SERVE,
        CANON_DUAL_RES_PRODUCE,
        CANON_TWO_ZONES,
        CANON_TRI_CYCLE
    };
    Constraints CONS;

    /////////////////
    // finishNetwork
    /////////////////

    TEST(GreedyEdgeListTest, FinishNetwork_Canon){
        // check canonical exaples
        for(const Network& net : CANON_NETS) {
            GreedyEdgeList solv(net, ALL_COSTS, CONS);
            EXPECT_TRUE(
                CONS(
                    solv.finishNetwork(
                        solv.getNet()
                    )
                )
            );
        }
    }

    TEST(GreedyEdgeListTest, FinishNetwork_Extra){
        // for canonical example
        for(Network net : CANON_NETS) {
            // add extra
            net.addFactory(Factory(13, 12, ResourceList({
                {Resource::Copper, 1}, 
                {Resource::Fish, 1}
            })));
            GreedyEdgeList solv(net, ALL_COSTS, CONS);
            EXPECT_TRUE(
                CONS(
                    solv.finishNetwork(
                        solv.getNet()
                    )
                )
            );
        }
    }

    TEST(GreedyEdgeListTest, FinishNetwork_Random_GridSize){
        // check random networks with an increasing grid size and a constant number of factories
        const int numFacts = 30;
        for(int gridSize = 5; gridSize < MAX_LOC_COORD; gridSize++){
            Network net = randomNetwork(rand(), numFacts, gridSize);
            GreedyEdgeList solv(net, ALL_COSTS, CONS);
            EXPECT_TRUE(
                CONS(
                    solv.finishNetwork(
                        solv.getNet()
                    )
                )
            );
        }
    }

    TEST(GreedyEdgeListTest, FinishNetwork_Random_Add_Facts){
        // check random networks with increasing number of factories
        for (int f = 2; f < MAX_NET_SIZE; f++){
            Network net = randomNetwork(rand(), f, f*10);
            GreedyEdgeList solv(net, ALL_COSTS, CONS);
            EXPECT_TRUE(
                CONS(
                    solv.finishNetwork(
                        solv.getNet()
                    )
                )
            );
        }
    }

    // /////////////////
    // // spliceRoutes
    // /////////////////
    // TEST(GreedyEdgeListTest, SpliceRoutes_Canon){
    //     FactoryKey BASE_LINK(314251, 1242121);
    //     // check canonical exaples
    //     for(const Network& net : CANON_NETS) {
    //         GreedyEdgeList solv(net, ALL_COSTS, CONS);
    //         // get working solution with a bunch of routes
    //         Network nn = solv.finishNetwork(solv.getNet());

    //         // get number of routes in network
    //         size_t numRoutes = nn.getNumRoutes();

    //         while(numRoutes > 1) {
    //             // check for routes with stops in common
    //             // for each pair of routes

    //             FactoryKey link = BASE_LINK;
    //             size_t r1, r2;
    //             for(r1 = 0; link == BASE_LINK && r1 < numRoutes-1; r1++) {
    //                 for(r2 = r1+1; link == BASE_LINK && r2 < numRoutes; r2++) {
    //                     // find shared facts 
    //                     std::vector<FactoryKey> st1 = nn.getRouteStops(r1), st2 = nn.getRouteStops(r2);
    //                     for(auto fk = st1.begin(); link == BASE_LINK && fk != st1.end(); fk++) {
    //                         for(auto fk2 = st2.begin(); link == BASE_LINK && fk2 != st2.end(); fk2++) {
    //                             if(*fk == *fk2) {
    //                                 link = *fk;
    //                             }
    //                         }
    //                     }
    //                 }
    //             }
    //             // check if still has intersection
    //             if(link == BASE_LINK)
    //                 break;
    //             nn = solv.spliceRoutes(nn, r1-1, r2-1, link);
    //             numRoutes--;
    //             EXPECT_EQ(nn.getNumRoutes(), numRoutes);
    //             EXPECT_TRUE(CONS(nn));
    //         }
    //     }
    // }

    // TEST(GreedyEdgeListTest, SpliceRoutes_Extra){
    //     FactoryKey BASE_LINK(314251, 1242121);
    //     // check canonical exaples
    //     for(Network net : CANON_NETS) {
    //         // add extra
    //         net.addFactory(Factory(13, 12, ResourceList({
    //             {Resource::Copper, 1}, 
    //             {Resource::Fish, 1}
    //         })));
    //         GreedyEdgeList solv(net, ALL_COSTS, CONS);
    //         // get working solution with a bunch of routes
    //         Network nn = solv.finishNetwork(solv.getNet());

    //         // get number of routes in network
    //         size_t numRoutes = nn.getNumRoutes();

    //         while(numRoutes > 1) {
    //             // check for routes with stops in common
    //             // for each pair of routes

    //             FactoryKey link = BASE_LINK;
    //             size_t r1, r2;
    //             for(r1 = 0; link == BASE_LINK && r1 < numRoutes-1; r1++) {
    //                 for(r2 = r1+1; link == BASE_LINK && r2 < numRoutes; r2++) {
    //                     // find shared facts 
    //                     std::vector<FactoryKey> st1 = nn.getRouteStops(r1), st2 = nn.getRouteStops(r2);
    //                     for(auto fk = st1.begin(); link == BASE_LINK && fk != st1.end(); fk++) {
    //                         for(auto fk2 = st2.begin(); link == BASE_LINK && fk2 != st2.end(); fk2++) {
    //                             if(*fk == *fk2) {
    //                                 link = *fk;
    //                             }
    //                         }
    //                     }
    //                 }
    //             }
    //             // check if still has intersection
    //             if(link == BASE_LINK)
    //                 break;
    //             nn = solv.spliceRoutes(nn, r1-1, r2-1, link);
    //             numRoutes--;
    //             EXPECT_EQ(nn.getNumRoutes(), numRoutes);
    //             EXPECT_TRUE(CONS(nn));
    //         }
    //     }
    // }

    // TEST(GreedyEdgeListTest, SpliceRoutes_Random){
    //     FactoryKey BASE_LINK(314251, 1242121);
    //     // check several randoms
    //     for(int iter = 0; iter < 10; iter++) {
    //         Network net = randomNetwork();
    //         GreedyEdgeList solv(net, ALL_COSTS, CONS);
    //         // get working solution with a bunch of routes
    //         Network nn = solv.finishNetwork(solv.getNet());

    //         // get number of routes in network
    //         size_t numRoutes = nn.getNumRoutes();

    //         while(numRoutes > 1) {
    //             // check for routes with stops in common
    //             // for each pair of routes

    //             FactoryKey link = BASE_LINK;
    //             size_t r1, r2;
    //             for(r1 = 0; link == BASE_LINK && r1 < numRoutes-1; r1++) {
    //                 for(r2 = r1+1; link == BASE_LINK && r2 < numRoutes; r2++) {
    //                     // find shared facts 
    //                     std::vector<FactoryKey> st1 = nn.getRouteStops(r1), st2 = nn.getRouteStops(r2);
    //                     for(auto fk = st1.begin(); link == BASE_LINK && fk != st1.end(); fk++) {
    //                         for(auto fk2 = st2.begin(); link == BASE_LINK && fk2 != st2.end(); fk2++) {
    //                             if(*fk == *fk2) {
    //                                 link = *fk;
    //                             }
    //                         }
    //                     }
    //                 }
    //             }
    //             // check if still has intersection
    //             if(link == BASE_LINK)
    //                 break;
    //             nn = solv.spliceRoutes(nn, r1-1, r2-1, link);
    //             numRoutes--;
    //             EXPECT_EQ(nn.getNumRoutes(), numRoutes);
    //             EXPECT_TRUE(CONS(nn));
    //         }
    //     }
    // }



    /////////////////
    // solve
    /////////////////
    const size_t NUM_ITERS = 10;

    TEST(GreedyEdgeListTest, Solve_Canon_Valid){
        // check canonical exaples
        for(const Network& net : CANON_NETS) {
            GreedyEdgeList solv(net, ALL_COSTS, CONS);
            EXPECT_TRUE(CONS(solv.solve()));
        }
    }
    TEST(GreedyEdgeListTest, Solve_Canon_Improved){
        // check canonical exaples
        for(const Network& net : CANON_NETS) {
            GreedyEdgeList solv(net, ALL_COSTS, CONS);
            Cost init_cost = ALL_COSTS(solv.finishNetwork(solv.getNet()));
            EXPECT_TRUE(ALL_COSTS(solv.solve()) <= init_cost);
        }
    }
    
    #include <iostream>
    TEST(GreedyEdgeListTest, Solve_Random_GridSize){
        // check random networks with an increasing grid size and a constant number of factories
        const int numFacts = 30;
        for(int gridSize = 5; gridSize < MAX_LOC_COORD; gridSize *= 1.5){
            size_t s = rand();
            std::cout << s << ", " << gridSize << std::endl;
            Network net = randomNetwork(s, numFacts, gridSize);
            GreedyEdgeList solv(net, ALL_COSTS, CONS);
            Cost init_cost = ALL_COSTS(solv.finishNetwork(solv.getNet()));
            Network solved = solv.solve();
            EXPECT_TRUE(CONS(solved));
            EXPECT_TRUE(ALL_COSTS(solved) <= init_cost);
        }
    }

    TEST(GreedyEdgeListTest, Solve_Random_Add_Facts){
        // check random networks with increasing number of factories
        for (int f = 2; f < MAX_NET_SIZE; f += 10){
            size_t s = rand();
            std::cout << s << ", " << f << std::endl;
            Network net = randomNetwork(s, f, f*10);
            GreedyEdgeList solv(net, ALL_COSTS, CONS);
            Cost init_cost = ALL_COSTS(solv.finishNetwork(solv.getNet()));
            Network solved = solv.solve();
            EXPECT_TRUE(CONS(solved));
            EXPECT_TRUE(ALL_COSTS(solved) <= init_cost);
        }
    }
}