/*
Canonical Examples unit test
Author: Mason Paladino
12/2/22

This file tests the Constraints object, defined in Constraints.h
*/

/////////////////
// Includes
/////////////////

#include <gtest/gtest.h>
#include "../headers/solutions/CanonicalExamples.h"
#include "../headers/solutions/Constraints.h"

/////////////////
// tests
/////////////////

//Test good network for solution constraints
//Check Twice
namespace CanonicalExamplesTest{
    TEST(CanonicalExamplesTest, RandomNetworkTest){
        for(int i = 0; i < 50; i++){
            Network net = randomNetwork();
            EXPECT_TRUE(net.getNumFactories() > 1);
            Constraints con;
            EXPECT_TRUE(con.isValidNetwork(net));
        }
    }

    TEST(CanonicalExamplesTest, SameSeedNetworks){
        auto seed = time(0);
        Network net1 = randomNetwork(seed);
        Network net2 = randomNetwork(seed);
        EXPECT_EQ(net1.getNumFactories(), net2.getNumFactories());

        auto it1 = net1.factCBegin();
        auto it2 = net2.factCBegin();
        while(it1 != net1.factCEnd()){
            Factory f1 = it1->second;
            Factory f2 = it2->second;
            EXPECT_EQ(f1.getLoc(), f2.getLoc());
            EXPECT_EQ(f1.getBaseQuants(), f2.getBaseQuants());
            it1++; it2++;
        }
    }

    TEST(CanonicalExamplesTest, InvalidNetwork){
        Network net1 = randomNetwork(time(0), 10000, 10);
        EXPECT_EQ(net1.getNumFactories(), 0);
        Network net2 = randomNetwork(time(0), 1);
        EXPECT_EQ(net2.getNumFactories(), 0);
    }
}