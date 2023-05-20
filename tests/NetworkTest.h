/*
Network Unit test
Author: Andrew Bergman, Mason Paladino
12/5/22

This contains the Unit tests for the Network class.
*/

/////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "../headers/data/Network.h"


/////////////////////////////////////////////////////////////////
// tests
/////////////////////////////////////////////////////////////////

namespace NetworkTest
{
    const int generalProduction = 1000;
    const ResourceList makeAll = ResourceList(
        {
            {Resource(0), generalProduction},
            {Resource(1), generalProduction},
            {Resource(2), generalProduction},
            {Resource(3), generalProduction},
            {Resource(4), generalProduction},
            {Resource(5), generalProduction},
            {Resource(6), generalProduction},
            {Resource(7), generalProduction},
            {Resource(8), generalProduction},
            {Resource(9), generalProduction},
            {Resource(10), generalProduction},
            {Resource(11), generalProduction},
        });
    const ResourceList eatAll = ResourceList(
        {
            {Resource(0), -generalProduction},
            {Resource(1), -generalProduction},
            {Resource(2), -generalProduction},
            {Resource(3), -generalProduction},
            {Resource(4), -generalProduction},
            {Resource(5), -generalProduction},
            {Resource(6), -generalProduction},
            {Resource(7), -generalProduction},
            {Resource(8), -generalProduction},
            {Resource(9), -generalProduction},
            {Resource(10), -generalProduction},
            {Resource(11), -generalProduction},
        });
    const ResourceList eatHalf = ResourceList(
        {
            {Resource(0), -generalProduction/2},
            {Resource(1), -generalProduction/2},
            {Resource(2), -generalProduction/2},
            {Resource(3), -generalProduction/2},
            {Resource(4), -generalProduction/2},
            {Resource(5), -generalProduction/2},
            {Resource(6), -generalProduction/2},
            {Resource(7), -generalProduction/2},
            {Resource(8), -generalProduction/2},
            {Resource(9), -generalProduction/2},
            {Resource(10), -generalProduction/2},
            {Resource(11), -generalProduction/2},
        });
        const Location WA(0,0), ID(3,5), OR(-10,-10), CA(-9, -30), NP(100,6);

        const std::vector<Factory> santaStops = {
            Factory(ID, makeAll),
            Factory(WA, eatAll),
            Factory(NP)
        };

        const std::vector<Factory> allStops = {
            Factory(WA, makeAll),
            Factory(ID, eatAll),
            Factory(OR, makeAll),
            Factory(CA, eatAll),
            Factory(NP, eatHalf)
        };

        const std::vector<Factory> allJunctions = {
            Factory(WA),
            Factory(ID),
            Factory(OR),
            Factory(CA),
            Factory(NP)
        };

        const Route santaTrip(
            (PairList<Location, ResourceList>)
            {
                {ID, makeAll},
                {WA, eatAll},
                {NP, ResourceList()}
            });

        const Route allStopsStates(
            (PairList<Location, ResourceList>)
            {
                {WA, makeAll},
                {ID, eatAll},
                {OR, makeAll},
                {CA, eatAll}
            });
        const Route allStopsEverywhere(
            (PairList<FactoryKey, ResourceList>)
            {
                {WA, makeAll},
                {ID, eatAll},
                {OR, makeAll},
                {CA, eatAll},
                {NP, eatHalf}
            });
    // constructors
    TEST(NetworkTest, DefaultConstructor)
    {
        Network natalia;
        EXPECT_EQ(natalia.getNumFactories(), 0); // verify network has expected properties
        EXPECT_EQ(natalia.getNumJunctions(), 0);
        EXPECT_EQ(natalia.getNumRoutes(), 0);
    }

    TEST(NetworkTest, FactoryListConstructor)
    {
        Network natalia(santaStops);
        EXPECT_EQ(natalia.getNumFactories(), santaStops.size()-1); // verify network has expected properties
        EXPECT_EQ(natalia.getNumJunctions(), santaStops.size()-2);
        EXPECT_EQ(natalia.getNumRoutes(), 0);
        EXPECT_EQ(natalia.getPlace(ID).getBaseQuants(), makeAll);
        EXPECT_EQ(natalia.getPlace(WA).getBaseQuants(), eatAll);
        EXPECT_EQ(natalia.getPlace(NP).getBaseQuants(), ResourceList());
    }

    // test Getter functions
    TEST(NetworkTest, GetNumFactories){
        Network harold(allStops);
        EXPECT_EQ(harold.getNumFactories(), allStops.size());
    }
    
    TEST(NetworkTest, GetNumJunctions)
    {
        Network natalia(allJunctions);
        EXPECT_EQ(natalia.getNumJunctions(), allJunctions.size());
    }

    TEST(NetworkTest, GetNumRoutes){
        Network natalia(santaStops);
        EXPECT_EQ(natalia.getNumRoutes(), 0);
        EXPECT_TRUE(natalia.addRoute(santaTrip));
        EXPECT_EQ(natalia.getNumRoutes(), 1);
    }

    TEST(NetworkTest, Getters){
        Network jerry(santaStops);
        EXPECT_TRUE(jerry.addRoute(santaTrip));
        Factory fact(ID, makeAll);
        EXPECT_EQ(jerry.getPlace(ID).getLoc(), fact.getLoc());
        EXPECT_EQ(jerry.getPlace(ID).getBaseQuants(), fact.getBaseQuants());
        EXPECT_EQ(jerry.getRoute(0), santaTrip);
    }

    // Test Adding Routes
    TEST(NetworkTest, AddRouteSuccess)
    {
        Network natalia(allStops);
        Route r(WA, ID, makeAll, eatAll);
        EXPECT_TRUE(natalia.addRoute(r));
        EXPECT_TRUE(natalia.addRoute(OR, CA, makeAll, eatAll));
        EXPECT_TRUE(natalia.addRoute((std::vector<FactoryKey>){ID,OR}));
        EXPECT_TRUE(natalia.addRoute((PairList<FactoryKey, ResourceList>){{NP, ResourceList()},{OR, ResourceList()}}));
        EXPECT_EQ(natalia.getNumRoutes(), 4);
    }

    TEST(NetworkTest, AddRouteFail){
        Network newton(santaStops);
        EXPECT_EQ(newton.getNumRoutes(), 0);
        EXPECT_TRUE(newton.addRoute(santaTrip));
        EXPECT_EQ(newton.getNumRoutes(), 1);
        EXPECT_FALSE(newton.addRoute(WA, NP, eatHalf, eatAll));
        EXPECT_EQ(newton.getNumRoutes(), 1);
        EXPECT_FALSE(newton.addRoute((std::vector<FactoryKey>){WA, CA, ID}, (std::vector<ResourceList>){eatAll, makeAll}));
        EXPECT_EQ(newton.getNumRoutes(), 1);
    }

    // Test ErasePlace 
    TEST(NetworkTest, ErasePlaceSuccess)
    {
        Network pascal(allStops);
        EXPECT_TRUE(pascal.addRoute(allStopsStates));
        EXPECT_EQ(pascal.getRoute(0).size(), allStopsStates.size());
        EXPECT_EQ(pascal.getNumFactories() + pascal.getNumJunctions(), allStops.size());
        EXPECT_TRUE(pascal.erasePlace(WA));
        EXPECT_NE(pascal.getNumFactories() + pascal.getNumJunctions(), allStops.size());
        EXPECT_NE(pascal.getRoute(0).size(), allStopsStates.size());
    }

    TEST(NetworkTest, ErasePlaceFail){
        Network charles(santaStops);
        EXPECT_TRUE(charles.addRoute(santaTrip));
        EXPECT_FALSE(charles.erasePlace(CA));
    }
    
    // Test Route Getter funcitons
    TEST(NetworkTest, GetRouteStops){
        Network randy(allStops);
        randy.addRoute(allStopsEverywhere);
        std::vector<FactoryKey> keys = randy.getRouteStops(0);
        EXPECT_EQ(keys.size(), allStopsEverywhere.size());
        for (int i = 0; i < keys.size(); i++)
        {
            EXPECT_GT(allStopsEverywhere.findStop(keys[i]),-1);
        }
    }
    
    TEST(NetworkTest, GetStopSuccess)
    {
        Network nathan(allStops);
        Route r = allStopsEverywhere;
        nathan.addRoute(r);
        for(int i = 0; i < r.size(); i++)
        {
            EXPECT_EQ(nathan.getStop(0,i), r[i]);
        }
    }

    TEST(NetworkTest, GetStopFail)
    {
        Network georgeWashington(allStops);
        georgeWashington.addRoute(allStopsEverywhere);
        EXPECT_THROW(georgeWashington.getStop(0, allStopsEverywhere.size()), std::out_of_range);
    }

    TEST(NetworkTest, GetStopCommandSuccess)
    {
        Network jamesMadison(allStops);
        jamesMadison.addRoute(allStopsEverywhere);
        for (int i = 0; i < allStopsEverywhere.size(); i++)
        {
            EXPECT_EQ(jamesMadison.getStopCommand(0,i), allStopsEverywhere.getResources(i));
        }
    }
        

    // Test add/Erase factory
    TEST(NetworkTest, AddFactory){
        Network jimbo;
        Factory fact(WA, eatHalf);
        EXPECT_EQ(jimbo.getNumFactories(), 0);
        jimbo.addFactory(fact);
        EXPECT_EQ(jimbo.getNumFactories(), 1);
        jimbo.addFactory(ID, makeAll);
        EXPECT_EQ(jimbo.getNumFactories(), 2);
        jimbo.addFactory((Coord)3, (Coord)4, eatAll);
        EXPECT_EQ(jimbo.getNumFactories(), 3);
    }

    TEST(NetworkTest, EraseFactory){
        Network tempest(allStops);
        EXPECT_TRUE(tempest.eraseFactory(tempest.getPlace(ID)));
        EXPECT_THROW(tempest.getPlace(ID), std::out_of_range);
        EXPECT_FALSE(tempest.eraseFactory(ID));
    }

    TEST(NetworkTest, EraseFactoryKey){
        Network maverick(santaStops);
        EXPECT_TRUE(maverick.eraseFactory(ID));
        EXPECT_THROW(maverick.getPlace(ID),std::out_of_range);
        EXPECT_FALSE(maverick.eraseFactory(ID));
    }

    // Test add/erase junction
    TEST(NetworkTest, CreateJunction){
        Network wombat;
        EXPECT_TRUE(wombat.createJunct(WA));
        EXPECT_EQ(wombat.getNumJunctions(), 1);
        EXPECT_TRUE(wombat.createJunct(4, 5));
        EXPECT_EQ(wombat.getNumJunctions(), 2);
    }

    TEST(NetworkTest, EraseJunction){
        Network elGuapo(santaStops);
        EXPECT_TRUE(elGuapo.createJunct(OR));
        EXPECT_TRUE(elGuapo.eraseJunction(OR));
        EXPECT_THROW(elGuapo.getPlace(OR), std::out_of_range);
        EXPECT_FALSE(elGuapo.eraseJunction(WA));
        EXPECT_FALSE(elGuapo.eraseJunction(OR));
    }
    
    // Test eraseRoute
    TEST(NetworkTest, EraseRoute)
    {
        Network abby(allStops);
        Route r = allStopsStates;
        abby.addRoute(r);
        EXPECT_EQ(abby.getNumRoutes(), 1);
        // abby should now have 4 stations whose Resources have all been picked up or satisfied.
        for (int i = 0; i < r.size(); i++)
        {
            EXPECT_EQ(abby.getPlace(abby.getStop(0, i)).getUnallocated(), ResourceList());
        }
        // erase the route, then check that the state of the Network has been reset.
        EXPECT_TRUE(abby.eraseRoute(0));
        EXPECT_EQ(abby.getNumRoutes(), 0);
        for (int i = 0; i < r.size(); i++)
        {
            EXPECT_EQ(abby.getPlace(r[i]).getUnallocated(), allStops[i].getUnallocated());
        }
    }

    // Test getDeficit
    TEST(NetworkTest, GetDeficit)
    {
        Network dennis(santaStops);
        // when the network is created, it should have one factory with a deficit, WA
        EXPECT_EQ(dennis.getDeficit().size(), 1);
        EXPECT_EQ(dennis.getDeficit()[0], WA);
        // after we add a route that satisfies the deficit, there should no longer be a deficit.
        dennis.addRoute(santaTrip);
        EXPECT_EQ(dennis.getDeficit().size(), 0);
    }

    // Test getSurplus
    TEST(NetworkTest, GetSurplus)
    {
        Network sally(santaStops);
        std::vector<FactoryKey> sallysSurplus = sally.getSurplus();
        // validate that we get the surplus that we expect
        EXPECT_EQ(sallysSurplus.size(), 1);
        EXPECT_EQ(sallysSurplus[0], ID);
        // after we add a route, we expect that there is no longer a surplus.
        sally.addRoute(santaTrip);
        sallysSurplus = sally.getSurplus();
        EXPECT_EQ(sallysSurplus.size(), 0);
    }

    // Test Route manipulation functions: addStop, setStopCommand, and dropStop
    TEST(NetworkTest, RouteInterfaceFunctions)
    {
        Network darthVader(allStops);
        darthVader.addRoute(WA, ID);
        // try to add a stop to the route
        EXPECT_EQ(darthVader.getRoute(0).size(), 2); 
        EXPECT_TRUE(darthVader.addStop(0, CA, darthVader.getRoute(0).size()));
        EXPECT_EQ(darthVader.getRoute(0).size(), 3);
        EXPECT_EQ(darthVader.getStop(0, darthVader.getRoute(0).size()-1), CA);

        // try and set a new command to a stop
        EXPECT_EQ(darthVader.getStopCommand(0, darthVader.getRoute(0).size()-1), ResourceList());
        EXPECT_TRUE(darthVader.setStopCommand(0, darthVader.getRoute(0).size()-1, eatAll));
        EXPECT_EQ(darthVader.getStopCommand(0, darthVader.getRoute(0).size()-1), eatAll);

        // try and drop a stop
        EXPECT_EQ(darthVader.getRoute(0).size(), 3);
        EXPECT_TRUE(darthVader.dropStop(0, CA));
        EXPECT_EQ(darthVader.getRoute(0).size(), 2);

    }

    TEST(NetworkTest, HasPlaceTest)
    {
        Network gilgamesh(santaStops);
        EXPECT_TRUE(gilgamesh.hasPlace(WA));
        EXPECT_TRUE(gilgamesh.hasPlace(NP));
        EXPECT_TRUE(gilgamesh.hasPlace(ID));
        EXPECT_FALSE(gilgamesh.hasPlace(CA));
        EXPECT_FALSE(gilgamesh.hasPlace(OR));
    }
}// namespace NetworkTest