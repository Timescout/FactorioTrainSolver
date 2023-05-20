/*
Constraints unit test
Author: Andrew Bergman
12/2/22

This file tests the Constraints object, defined in Constraints.h
*/

/////////////////
// Includes
/////////////////

#include <gtest/gtest.h>
#include "../headers/solutions/Constraints.h"
#include "TestSetup.h"
#include "../headers/solutions/CanonicalExamples.h"

/////////////////
// tests
/////////////////

//Test good network for solution constraints
//Check Twice
namespace ConstraintsTest {
    const std::list<Network> CANON_NETS = {
        CANON_BASIC,
        CANON_DUAL_SERVE,
        CANON_DUAL_RES_PRODUCE,
        CANON_TWO_ZONES,
        CANON_TRI_CYCLE
    };
    const Constraints cons;

    /////////////////
    // isValidNetwork
    /////////////////

    TEST(ConstraintsTest, ValidNetworks_EmptyNet){
        EXPECT_TRUE(cons.isValidNetwork(Network()));
    }

    TEST(ConstraintsTest, ValidNetworks_Exact){
        // check cannonical exaples
        for(const Network& n : CANON_NETS)
            EXPECT_TRUE(cons.isValidNetwork(n));
    }

    TEST(ConstraintsTest, ValidNetworks_Extra){
        // for canonical example
        for(Network net : CANON_NETS) {
            // add extra
            net.addFactory(Factory(13, 12, ResourceList({
                {Resource::Copper, 1}, 
                {Resource::Fish, 1}
            })));
            // expect still valid
            EXPECT_TRUE(cons.isValidNetwork(net));
        }
    }

    TEST(ConstraintsTest, ValidNetworks_Deficit){
        // for canonical example
        for(Network net : CANON_NETS) {
            // add extra
            net.addFactory(Factory(13, 12, ResourceList({
                {Resource::Copper, -1}, 
                {Resource::Fish, -1}
            })));
            // expect still valid
            EXPECT_FALSE(cons.isValidNetwork(net));
        }
    }

    /////////////////
    // checkSatisfied
    /////////////////

    TEST(ConstraintsTest, Satisfied_EmptyNet){
        EXPECT_TRUE(cons.checkSatisfied(Network()));
    }

    TEST(ConstraintsTest, Satisfied_NoRoutes){
        // check cannonical exaples
        for(const Network& n : CANON_NETS)
            EXPECT_FALSE(cons.checkSatisfied(n));
    }

    TEST(ConstraintsTest, Satisfied_EveryFact){
        // check cannonical exaples
        for(Network net : CANON_NETS) {
            PairList<FactoryKey, ResourceList> r_lis;
            // auto = std::map<FactoryKey, Factory, LocationCompare>::const_iterator
            for (auto fact_it = net.factCBegin(); fact_it != net.factCEnd(); fact_it++)
                r_lis.emplace_back(fact_it->first, fact_it->second.getBaseQuants());
            // add extra
            net.addRoute(r_lis);
            // expect still valid
            EXPECT_TRUE(cons.checkSatisfied(net));
        }
    }

    /////////////////
    // checkTwice
    /////////////////

    TEST(ConstraintsTest, Twice_EmptyNet){
        EXPECT_TRUE(cons.checkTwice(Network()));
    }

    TEST(ConstraintsTest, Twice_NoRoutes){
        // check cannonical exaples
        for(const Network& n : CANON_NETS)
            EXPECT_TRUE(cons.checkTwice(n));
    }

    TEST(ConstraintsTest, Twice_EveryFact){
        // check cannonical exaples
        for(Network net : CANON_NETS) {
            PairList<FactoryKey, ResourceList> r_lis;
            // auto = std::map<FactoryKey, Factory, LocationCompare>::const_iterator
            for (auto fact_it = net.factCBegin(); fact_it != net.factCEnd(); fact_it++)
                r_lis.emplace_back(fact_it->first, fact_it->second.getBaseQuants());
            // add extra
            net.addRoute(r_lis);
            // expect still valid
            EXPECT_TRUE(cons.checkTwice(net));
        }
    }

    TEST(ConstraintsTest, Twice_Trivial){
        // check cannonical exaples
        for(Network net : CANON_NETS) {
            // add self route
            EXPECT_TRUE(net.addRoute({
                {net.factCBegin()->first, ResourceList()},
                {net.factCBegin()->first, ResourceList()}
            }));
            // expect still valid
            EXPECT_FALSE(cons.checkTwice(net));
        }
    }

    TEST(ConstraintsTest, Twice_Start){
        // check cannonical exaples
        for(Network net : CANON_NETS) {
            // add self route
            EXPECT_TRUE(net.addRoute({
                {net.factCBegin()->first, ResourceList()},
                {net.factCBegin()->first, ResourceList()},
                {(--net.factCEnd())->first, ResourceList()}
            }));
            // expect still valid
            EXPECT_FALSE(cons.checkTwice(net));
        }
    }

    TEST(ConstraintsTest, Twice_End){
        // check cannonical exaples
        for(Network net : CANON_NETS) {
            // add self route
            EXPECT_TRUE(net.addRoute({
                {(--net.factCEnd())->first, ResourceList()},
                {net.factCBegin()->first, ResourceList()},
                {net.factCBegin()->first, ResourceList()}
            }));
            // expect still valid
            EXPECT_FALSE(cons.checkTwice(net));
        }
    }

    TEST(ConstraintsTest, Twice_Wrap){
        // check cannonical exaples
        for(Network net : CANON_NETS) {
            // add self route
            EXPECT_TRUE(net.addRoute({
                {net.factCBegin()->first, ResourceList()},
                {(--net.factCEnd())->first, ResourceList()},
                {net.factCBegin()->first, ResourceList()}
            }));
            // expect still valid
            EXPECT_FALSE(cons.checkTwice(net));
        }
    }
}