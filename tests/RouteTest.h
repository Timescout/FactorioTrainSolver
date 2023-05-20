#include <gtest/gtest.h>
#include "../headers/data/Route.h"
#include <vector>
#include <exception>

// Constants used throughout testing Route
namespace RouteTest
{
    const Location first(17,18), second(29, 29), third(31, 31); // used for factory indexes
    const Dist d_1_2 = dist(first, second), d_2_3 = dist(second, third), d_1_3 = dist(first, third); // used for metrics
    const int amount = 5; // used for amounts
    ResourceList RLCopper = ResourceList({{Resource::Copper, amount}});
    ResourceList RLCopperConsume = ResourceList({{Resource::Copper, -amount}});
    const Route twoStops(PairList<FactoryKey, ResourceList>({
        {first,  RLCopper},
        {second, RLCopperConsume}
    })),      threeStops(PairList<FactoryKey, ResourceList>({
        {first,  ResourceList()},
        {second, RLCopper},
        {third,  RLCopperConsume}
    })),    backAndForth(PairList<FactoryKey, ResourceList>({
        {first,  RLCopper},
        {second, RLCopperConsume},
        {first,  RLCopper},
        {third,  RLCopperConsume}
    }));

    // test constructors
    TEST(RouteTest, ListConstructorSuccess)
    {
        PairList<FactoryKey, ResourceList> list
        {
            {first, ResourceList()},
            {second, RLCopper}
        };
        EXPECT_NO_THROW(
            Route r(list);
            EXPECT_EQ(r[0], first);
            EXPECT_EQ(r[1], second);
            EXPECT_EQ(r.getResources(0), ResourceList());
            EXPECT_EQ(r.getResources(1), RLCopper)
            );
    }

    TEST(RouteTest, ListConstructorNoFacts) {
        // https://stackoverflow.com/a/40561685
        // this tests _that_ the expected exception is thrown
        EXPECT_THROW({
            try
            {
                // empty pairlist, should throw
                Route r = Route(PairList<FactoryKey, ResourceList>());
            }
            catch( const std::invalid_argument& e )
            {
                // and this tests that it has the correct message
                EXPECT_STREQ( "Route must be initialized with at least 2 factories", e.what() );
                throw;
            }
        }, std::invalid_argument);
    }

    TEST(RouteTest, TwoIndexConstructor) {
        EXPECT_NO_THROW(
            Route r(first, second, RLCopper);
            EXPECT_EQ(r[0], first);
            EXPECT_EQ(r[1], second);
            EXPECT_EQ(r.getResources(0), RLCopper);
            EXPECT_EQ(r.getResources(1), ResourceList());
            );
    }

    TEST(RouteTest, TwoCycle_ProduceConsume) {
        EXPECT_NO_THROW(
            Route r(first, second, RLCopper, RLCopperConsume);
            EXPECT_EQ(r[0], first);
            EXPECT_EQ(r[1], second);
            EXPECT_EQ(r.getResources(0), RLCopper);
            EXPECT_EQ(r.getResources(1), RLCopperConsume);
            );
    }

    // test addStop
    TEST(RouteTest, AddStopEnd)
    {
        Route r(first, second);
        r.addStop(third);
        EXPECT_EQ(r[0], first);
        EXPECT_EQ(r[1], second);
        EXPECT_EQ(r[2], third);
    }

    TEST(RouteTest, AddStopBeginning)
    {
        Route r(second, third);
        r.addStop(first, 0);
        EXPECT_EQ(r[0], first);
        EXPECT_EQ(r[1], second);
        EXPECT_EQ(r[2], third);
    }

    TEST(RouteTest, AddStopMiddle)
    {
        Route r(first, third);
        r.addStop(second, 1);
        EXPECT_EQ(r[0], first);
        EXPECT_EQ(r[1], second);
        EXPECT_EQ(r[2], third);
    }

    TEST(RouteTest, AddStopPastEnd)
    {
        Route r(first, second);
        r.addStop(third, 100);
        EXPECT_EQ(r[2], third); // trying to add a stop out of range just adds it to the end.
    }

    // test dropStop
    TEST(RouteTest, DropStopSucess)
    {
        Route r = threeStops;
        EXPECT_EQ(r.size(), 3);
        r.dropStop(first);
        EXPECT_EQ(r.size(), 2);
        EXPECT_EQ(r[0], second);
        EXPECT_EQ(r[1], third);
        EXPECT_EQ(r.getResources(0), RLCopper);
        EXPECT_EQ(r.getResources(1), RLCopperConsume);
    }

    TEST(RouteTest, DropStopSucessMiddle)
    {
        Route r = threeStops;
        EXPECT_EQ(r.size(), 3);
        r.dropStop(second, 1);
        EXPECT_EQ(r.size(), 2);
        EXPECT_EQ(r[0], first);
        EXPECT_EQ(r[1], third);
        EXPECT_EQ(r.getResources(0), ResourceList());
        EXPECT_EQ(r.getResources(1), RLCopperConsume);
    }

    TEST(RouteTest, DropStopSucessEnd)
    {
        Route r = threeStops;
        EXPECT_EQ(r.size(), 3);
        r.dropStop(third, 2);
        EXPECT_EQ(r.size(), 2);
        EXPECT_EQ(r[0], first);
        EXPECT_EQ(r[1], second);
        EXPECT_EQ(r.getResources(0), ResourceList());
        EXPECT_EQ(r.getResources(1), RLCopper);
    }

    TEST(RouteTest, DropSecondInstance)
    {
        Route r = backAndForth;
        EXPECT_EQ(r.size(), 4);
        r.dropStop(first, 1); // drop the second instance of the stop 'first'
        EXPECT_EQ(r[0], first);
        EXPECT_EQ(r[1], second);
        EXPECT_EQ(r[2], third);
        EXPECT_EQ(r.getResources(0), RLCopper);
        EXPECT_EQ(r.getResources(1), RLCopperConsume);
        EXPECT_EQ(r.getResources(2), RLCopperConsume);
    }

    TEST(RouteTest, DropStopSizeConstraint)
    {
        Route r = twoStops;
        EXPECT_FALSE(r.dropStop(first));
        EXPECT_FALSE(r.dropStop(second));
        EXPECT_EQ(r, twoStops);
    }

    TEST(RouteTest, DropStopFailNoStop)
    {
        Route r = twoStops;
        EXPECT_FALSE(r.dropStop(third));
        EXPECT_EQ(r, twoStops);
    }

    TEST(RouteTest, DropStopFailOutOfRange)
    {
        Route r = threeStops;
        EXPECT_THROW(
            try
            {
                r.dropStop(first, 4);
            }
            catch (const std::out_of_range& e) 
            {
                EXPECT_STREQ( "Attempted to drop factoryIndex outside of Route range", e.what() );
                throw;
            },
            std::out_of_range);

        EXPECT_EQ(r, threeStops);
    }

    // test operator[]()
    TEST(RouteTest, AccessOperatorTestSuccess)
    {
        Route r = threeStops;
        EXPECT_TRUE(r[0] == first);
        EXPECT_TRUE(r[1] == second);
        EXPECT_TRUE(r[2] == third);
    }

    TEST(RouteTest, AccessOperatorTestFail)
    {
        EXPECT_THROW(
            try
            {
                Route r = threeStops;
                FactoryKey f = r[3];
            }
            catch(const std::out_of_range& e)
            {
                EXPECT_STREQ( "Route stop out of range", e.what() );
                throw;
            },
            std::out_of_range);
    }

    // test getResources
    TEST(RouteTest, GetResourcesSuccess)
    {
        Route r = twoStops;
        EXPECT_EQ(r.getResources(0), RLCopper);
        EXPECT_EQ(r.getResources(1), RLCopperConsume);
    }

    TEST(RouteTest, GetResourcesFailOutOfRange)
    {
        EXPECT_THROW(
            try
            {
                Route r = twoStops;
                r.getResources(2);
            }
            catch(const std::out_of_range& e)
            {
                EXPECT_STREQ( "Attempted to getResources outside of Route range", e.what());
                throw;
            }, std::out_of_range );
    }

    // test getResourceAmount
    TEST(RouteTest, GetResourceAmountSuccess)
    {
        Route r = twoStops;
        EXPECT_EQ(r.getResourceAmount(0, Resource::Copper), RLCopper[Resource::Copper]);
        EXPECT_EQ(r.getResourceAmount(1, Resource::Copper), RLCopperConsume[Resource::Copper]);
    }

    TEST(RouteTest, GetResourceAmountOutOfRange)
    {
        EXPECT_THROW(
            try
            {
                Route r = twoStops;
                r.getResourceAmount(2, Resource::Copper);
            }
            catch(const std::out_of_range& e)
            {
                EXPECT_STREQ( "Attempted to getResourcesAmount outside of Route range", e.what());
                throw;
            }, std::out_of_range );
    }

    // test getNetResources
    TEST(RouteTest, GetNetResources)
    {
        EXPECT_EQ(twoStops.getNetResources(), RLCopper + RLCopperConsume);
    }

    // test size
    TEST(RouteTest, Size)
    {
        EXPECT_EQ(twoStops.size(), 2);
        EXPECT_EQ(threeStops.size(), 3);
        Route r = threeStops;
        r.dropStop(r[0]); // check that size correctly updates.
        EXPECT_EQ(r.size(), 2);
    }

    // test operator==
    TEST(RouteTest, ComparisonOperator)
    {
        Route a = threeStops;
        EXPECT_TRUE(a == threeStops);
        EXPECT_FALSE(twoStops == threeStops);
    }

    // test findStop
    TEST(RouteTest, HasStopTrue)
    {
        EXPECT_EQ(twoStops.findStop(first), 0);
        EXPECT_EQ(twoStops.findStop(second), 1);
    }

    TEST(RouteTest, HasStopFalse)
    {
        EXPECT_EQ(twoStops.findStop(third), -1);
    }

    // test SetResourceList
    TEST(RouteTest, SetResourceList)
    {
        Route r = twoStops;
        EXPECT_NO_THROW(r.setResourceList(0, ResourceList()));
        EXPECT_EQ(r.getResources(0), ResourceList());
        EXPECT_FALSE(r == twoStops);
    }

    TEST(RouteTest, SetResourceListFail)
    {
        EXPECT_THROW(
            try
            {
                Route r = twoStops;
                r.setResourceList(2, ResourceList());
            }
            catch(const std::out_of_range& e)
            {
                EXPECT_STREQ( "Attempted to setResourceList outside of Route range", e.what());
                throw;
            }, std::out_of_range );
    }

    TEST(RouteTest, SetResource)
    {
        Route r = twoStops;
        EXPECT_NO_THROW(r.setResource(1, Resource::Iron, amount));
        EXPECT_EQ(r.getResourceAmount(1, Resource::Iron), amount);
    }

    TEST (RouteTest, SetResourceFail)
    {
        EXPECT_THROW(
            try
            {
                Route r = twoStops;
                r.setResource(2, Resource::Iron, amount);
            }
            catch(const std::out_of_range& e)
            {
                EXPECT_STREQ( "Attempted to setResource outside of Route range", e.what());
                throw;
            }, std::out_of_range );
    }

    TEST(RouteTest, BasicMetricsTwoStops) {
        EXPECT_EQ(twoStops.getLength(), d_1_2 + d_1_2);
        EXPECT_EQ(twoStops.getNumStops(), 2);
    }

    TEST(RouteTest, BasicMetricsThreeStops) {
        EXPECT_EQ(threeStops.getLength(), d_1_2 + d_2_3 + d_1_3);
        EXPECT_EQ(threeStops.getNumStops(), 3);
    }

    TEST(RouteTest, BasicMetricsBackAndForth) {
        EXPECT_EQ(backAndForth.getLength(), d_1_2 + d_1_2 + d_1_3 + d_1_3);
        EXPECT_EQ(backAndForth.getNumStops(), 4);
    }

    TEST(RouteTest, CapMetricsTwoStops) {
        EXPECT_EQ(twoStops.getCarryover(), ResourceList());
        EXPECT_EQ(twoStops.getCarryTime(), d_1_2.toDouble() * amount);
        EXPECT_EQ(twoStops.getPeakCapacity(), amount);
    }

    TEST(RouteTest, CapMetricsThreeStops) {
        EXPECT_EQ(threeStops.getCarryover(), ResourceList());
        EXPECT_EQ(threeStops.getCarryTime(), d_2_3.toDouble() * amount);
        EXPECT_EQ(threeStops.getPeakCapacity(), amount);
    }

    TEST(RouteTest, CapMetricsBackAndForth) {
        EXPECT_EQ(backAndForth.getCarryover(), ResourceList());
        EXPECT_EQ(backAndForth.getCarryTime(), (d_1_2 + d_1_3).toDouble() * amount);
        EXPECT_EQ(backAndForth.getPeakCapacity(), amount);
    }

    TEST(RouteTest, CapMetricsTwoStopsAlmostCarry) {
        Route r(PairList<FactoryKey, ResourceList>({
            {first,  RLCopperConsume},
            {second, RLCopper}
        }));
        EXPECT_EQ(r.getCarryover(), ResourceList());
        EXPECT_EQ(r.getCarryTime(), d_1_2.toDouble() * amount);
        EXPECT_EQ(r.getPeakCapacity(), amount);
    }

    TEST(RouteTest, CapMetricsThreeStopsCarry) {
        Route r(PairList<FactoryKey, ResourceList>({
            {first,  ResourceList()},
            {second, RLCopperConsume},
            {third,  RLCopper}
        }));
        EXPECT_EQ(r.getCarryover(), RLCopper);
        EXPECT_EQ(r.getCarryTime(), (d_1_3 + d_1_2).toDouble() * amount);
        EXPECT_EQ(r.getPeakCapacity(), amount);
    }

    TEST(RouteTest, CapMetricsVariableCap) {
        const Quant copQ = 2, wireQ = 20, circQ = 1;
        const Location loca(0, 0), locb(0, 3), locc(1, 2), locd(3, 1), loce(4, 0);
        const ResourceList rla({
            {Resource::Circuits, -circQ*2},
            {Resource::Copper, copQ*4}
        }), rlb({
            {Resource::Copper, -copQ},
            {Resource::Wire, wireQ}
        }), rlc({
            {Resource::Copper, -copQ},
            {Resource::Wire, wireQ}
        }), rld({ 
            {Resource::Copper, -copQ},
            {Resource::Wire, -wireQ},
            {Resource::Circuits, circQ}
        }), rle({ 
            {Resource::Copper, -copQ},
            {Resource::Wire, -wireQ},
            {Resource::Circuits, circQ}
        });
        Route r((PairList<FactoryKey, ResourceList>){
            {loca, rla},
            {locb, rlb},
            {locc, rlc},
            {locd, rld},
            {loce, rle}
        });
        EXPECT_EQ(r.getCarryover(), ResourceList());
        double carry = dist(loca, locb).toDouble() * (copQ*4)
                     + dist(locb, locc).toDouble() * (copQ*3 + wireQ)
                     + dist(locc, locd).toDouble() * (copQ*2 + wireQ*2)
                     + dist(locd, loce).toDouble() * (copQ + wireQ + circQ)
                     + dist(loce, loca).toDouble() * (circQ*2);
        EXPECT_EQ(r.getCarryTime(), carry);
        EXPECT_EQ(r.getPeakCapacity(), copQ*2 + wireQ*2);
    }

    TEST(RouteTest, ReverseTwoStops) {
        Route r(twoStops);
        for(size_t i = 0; i < 3; i++) {
            EXPECT_EQ(r.getNetResources(), ResourceList());
            EXPECT_EQ(r.getNumStops(), 2);
            EXPECT_EQ(r.getLength(), d_1_2+d_1_2);
            EXPECT_EQ(r.getPeakCapacity(), amount);
            r.reverse();
        }
    }

    TEST(RouteTest, ReverseThreeStops) {
        Route r(threeStops);
        for(size_t i = 0; i < 3; i++) {
            EXPECT_EQ(r.getNetResources(), ResourceList());
            EXPECT_EQ(r.getNumStops(), 3);
            EXPECT_EQ(r.getLength(), d_1_2+d_2_3+d_1_3);
            EXPECT_EQ(r.getPeakCapacity(), amount);
            r.reverse();
        }
    }

    TEST(RouteTest, ReverseBackAndForth) {
        Route r(backAndForth);
        for(size_t i = 0; i < 3; i++) {
            EXPECT_EQ(r.getNetResources(), ResourceList());
            EXPECT_EQ(r.getNumStops(), 4);
            EXPECT_EQ(r.getLength(), d_1_2+d_1_2+d_1_3+d_1_3);
            EXPECT_EQ(r.getPeakCapacity(), amount);
            r.reverse();
        }
    }

    TEST(RouteTest, RotateTwoStops) {
        Route r(twoStops);
        for(size_t i = 0; i < 10; i++) {
            r.rotate(i);
            EXPECT_EQ(r.getNetResources(), ResourceList());
            EXPECT_EQ(r.getNumStops(), 2);
            EXPECT_EQ(r.getLength(), d_1_2+d_1_2);
            EXPECT_EQ(r.getPeakCapacity(), amount);
            EXPECT_EQ(r.getPeakCapacity(), amount);
            EXPECT_EQ(r.getCarryTime(), d_1_2.toDouble() * amount);
        }
    }

    TEST(RouteTest, RotateThreeStops) {
        Route r(threeStops);
        for(size_t i = 0; i < 10; i++) {
            r.rotate(i);
            EXPECT_EQ(r.getNetResources(), ResourceList());
            EXPECT_EQ(r.getNumStops(), 3);
            EXPECT_EQ(r.getLength(), d_1_2+d_2_3+d_1_3);
            EXPECT_EQ(r.getPeakCapacity(), amount);
            EXPECT_EQ(r.getCarryTime(), d_2_3.toDouble() * amount);
        }
    }

    TEST(RouteTest, RotateBackAndForth) {
        Route r(backAndForth);
        for(size_t i = 0; i < 10; i++) {
            r.rotate(i);
            EXPECT_EQ(r.getNetResources(), ResourceList());
            EXPECT_EQ(r.getNumStops(), 4);
            EXPECT_EQ(r.getLength(), d_1_2+d_1_2+d_1_3+d_1_3);
            EXPECT_EQ(r.getPeakCapacity(), amount);
            EXPECT_EQ(r.getCarryTime(), (d_1_2 + d_1_3).toDouble() * amount);
        }
    }

} // namespace RouteTest