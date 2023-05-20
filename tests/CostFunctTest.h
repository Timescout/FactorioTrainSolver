#include <gtest/gtest.h>
#include "../headers/solutions/CostFunct.h"
#include "../headers/solutions/CanonicalExamples.h"

#include <iostream>
#include <vector>

// size_t getNumJunctions(const Network& net) const;
// std::map<size_t, Dist> getTrackLengthMap(const Network& net) const;
// Dist getMaxLength(const Network& net) const;
// size_t getNumRoutes(const Network& net) const;
// double getMaxCarryTime(const Network& net) const;
// double getTotalCarryTime(const Network& net) const;
// Quant getMaxPeakCapacity(const Network& net) const;
// Quant getTotalPeakCapacity(const Network& net) const;
// Cost operator()(const Network& net) const;

namespace CostFunctTest {
    // don't change this, it'll probably break stuff
    TEST(CostFunctTest, CanonBasic) {
        Network net(CANON_BASIC);
        // route without carryover
        EXPECT_TRUE(net.addRoute(
            net.factCBegin()->first,
            (++net.factCBegin())->first,
            net.factCBegin()->second.getBaseQuants(), 
            (++net.factCBegin())->second.getBaseQuants())
        );
        Dist len = {2, 0};
        EXPECT_EQ(ALL_COSTS.getNumJunctions(net), 0);
        EXPECT_EQ(
            ALL_COSTS.getTrackLengthMap(net), 
            ((std::map<size_t, Dist>){{1, len}})
        );
        EXPECT_EQ(ALL_COSTS.getMaxLength(net), len);
        EXPECT_EQ(ALL_COSTS.getNumRoutes(net), 1);
        EXPECT_DOUBLE_EQ(ALL_COSTS.getMaxCarryTime(net), 1.0);
        EXPECT_DOUBLE_EQ(ALL_COSTS.getTotalCarryTime(net), 1.0);
        EXPECT_EQ(ALL_COSTS.getMaxPeakCapacity(net), 1);
        EXPECT_EQ(ALL_COSTS.getTotalPeakCapacity(net), 1);

        EXPECT_DOUBLE_EQ(ALL_COSTS(net), 6.2); // 2.0 + 2.0 + 1.0+ 0.5 + 0.1 + 0.5 + 0.1
    }

    TEST(CostFunctTest, CanonDualServe) {
        Network net(CANON_DUAL_SERVE);
        Location loca = net.factCBegin()->first, locb = (++net.factCBegin())->first, locc = (++++net.factCBegin())->first;
        ResourceList rla = net.factCBegin()->second.getBaseQuants(), rlb = (++net.factCBegin())->second.getBaseQuants(), rlc = (++++net.factCBegin())->second.getBaseQuants();
        Dist dab = dist(loca, locb), dbc = dist(locb, locc), dac = dist(loca, locc), 
             len = dab + dbc + dac;
        double carry = 3*dab.toDouble() + 1*dbc.toDouble();
        Quant cap = 3;
        // route with carryover
        EXPECT_TRUE(net.addRoute((PairList<FactoryKey, ResourceList>){
            {locb, rlb},
            {locc, rlc},
            {loca, rla}
        }));
        EXPECT_EQ(ALL_COSTS.getNumJunctions(net), 0);
        EXPECT_EQ(
            ALL_COSTS.getTrackLengthMap(net), 
            ((std::map<size_t, Dist>){{1, len}})
        );
        EXPECT_EQ(ALL_COSTS.getMaxLength(net), len);
        EXPECT_EQ(ALL_COSTS.getNumRoutes(net), 1);
        EXPECT_DOUBLE_EQ(ALL_COSTS.getMaxCarryTime(net), carry);
        EXPECT_DOUBLE_EQ(ALL_COSTS.getTotalCarryTime(net), carry);
        EXPECT_EQ(ALL_COSTS.getMaxPeakCapacity(net), cap);
        EXPECT_EQ(ALL_COSTS.getTotalPeakCapacity(net), cap);

        EXPECT_DOUBLE_EQ(ALL_COSTS(net),
            ALL_COSTS.weights[CostFunct::Metric::BaseTrackLength]*len.toDouble() + 
            ALL_COSTS.weights[CostFunct::Metric::MaxLength]*len.toDouble() + 
            ALL_COSTS.weights[CostFunct::Metric::NumRoutes]*1 + 
            ALL_COSTS.weights[CostFunct::Metric::MaxCarryTime]*carry + 
            ALL_COSTS.weights[CostFunct::Metric::TotalCarryTime]*carry + 
            ALL_COSTS.weights[CostFunct::Metric::MaxPeakCapacity]*cap + 
            ALL_COSTS.weights[CostFunct::Metric::TotalPeakCapacity]*cap
        );
    }

    TEST(CostFunctTest, CanonDualResProduce) {
        Network net(CANON_DUAL_RES_PRODUCE);
        Factory a(net.getPlace((Location){0, 0})), b(net.getPlace((Location){2, 1})), c(net.getPlace((Location){1, 2}));
        Dist dab = dist(a.getLoc(), b.getLoc()), dbc = dist(b.getLoc(), c.getLoc()), dac = dist(a.getLoc(), c.getLoc());
        // copper route
        EXPECT_TRUE(net.addRoute((PairList<FactoryKey, ResourceList>){
            {a.getLoc(), ResourceList({{Resource::Copper, -1}})},
            {b.getLoc(), ResourceList({{Resource::Copper, 1}})}
        }));
        // iron route
        EXPECT_TRUE(net.addRoute((PairList<FactoryKey, ResourceList>){
            {a.getLoc(), ResourceList({{Resource::Iron, -1}})},
            {c.getLoc(), ResourceList({{Resource::Iron, 1}})}
        }));
        // loop route
        EXPECT_TRUE(net.addRoute((PairList<FactoryKey, ResourceList>){
            {a.getLoc(), ResourceList({{Resource::Copper, -1}, {Resource::Iron, -1}})},
            {b.getLoc(), ResourceList({{Resource::Copper, 1}})},
            {c.getLoc(), ResourceList({{Resource::Iron, 1}})}
        }));
        EXPECT_EQ(ALL_COSTS.getNumJunctions(net), 0);
        EXPECT_EQ(
            ALL_COSTS.getTrackLengthMap(net), 
            ((std::map<size_t, Dist>){
                {1, dab+dac+dbc},
                {2, dab+dac}
            })
        );
        EXPECT_EQ(ALL_COSTS.getMaxLength(net), dab+dac+dbc);
        EXPECT_EQ(ALL_COSTS.getNumRoutes(net), 3);
        EXPECT_DOUBLE_EQ(ALL_COSTS.getMaxCarryTime(net), dbc.toDouble() + 2*dac.toDouble());
        EXPECT_DOUBLE_EQ(ALL_COSTS.getTotalCarryTime(net), dab.toDouble() + dbc.toDouble() + 3*dac.toDouble());
        EXPECT_EQ(ALL_COSTS.getMaxPeakCapacity(net), 2);
        EXPECT_EQ(ALL_COSTS.getTotalPeakCapacity(net), 4);

        EXPECT_DOUBLE_EQ(ALL_COSTS(net),
            ALL_COSTS.weights[CostFunct::Metric::BaseTrackLength]*(dab+dac+dab+dac+dbc).toDouble() + 
            ALL_COSTS.weights[CostFunct::Metric::SharedTrackLength]*(dab+dac).toDouble() + 
            ALL_COSTS.weights[CostFunct::Metric::MaxLength]*(dab+dac+dbc).toDouble() + 
            ALL_COSTS.weights[CostFunct::Metric::NumRoutes]*3 + 
            ALL_COSTS.weights[CostFunct::Metric::MaxCarryTime]*(dbc.toDouble() + 2*dac.toDouble()) + 
            ALL_COSTS.weights[CostFunct::Metric::TotalCarryTime]*(dab.toDouble() + dbc.toDouble() + 3*dac.toDouble()) + 
            ALL_COSTS.weights[CostFunct::Metric::MaxPeakCapacity]*2 + 
            ALL_COSTS.weights[CostFunct::Metric::TotalPeakCapacity]*4
        );
    }

    TEST(CostFunctTest, CanonTwoZones_Junctions) {
        Network net(CANON_TWO_ZONES);
        Location locaR(0, 0),  locbR(0, 2), 
                 locaL(100, 0),locbL(100, 2),
                 locjR(1, 1),  locjL(99, 1);
        Factory aR(net.getPlace(locaR)), 
                bR(net.getPlace(locbR)), 
                aL(net.getPlace(locaL)), 
                bL(net.getPlace(locbL));
        EXPECT_TRUE(net.createJunct(locjR)); 
        EXPECT_TRUE(net.createJunct(locjL));
        Dist daR = dist(locaR, locjR), daL = dist(locaL, locjL), 
             dbR = dist(locbR, locjR), dbL = dist(locbL, locjL),
             dJ  = dist(locjR, locjL);
        //double cA = 2*(daR+dJ+daL).toDouble() + 4*(daR+dJ+daL).toDouble(), cB = ;
        // route A
        EXPECT_TRUE(net.addRoute((PairList<FactoryKey, ResourceList>){
            {locaR, net.getPlace(locaR).getBaseQuants()},
            {locjR, ResourceList()},
            {locjL, ResourceList()},
            {locaL, net.getPlace(locaL).getBaseQuants()},
            {locjL, ResourceList()},
            {locjR, ResourceList()}
        }));
        // route B
        EXPECT_TRUE(net.addRoute((PairList<FactoryKey, ResourceList>){
            {locbR, net.getPlace(locbR).getBaseQuants()},
            {locjR, ResourceList()},
            {locjL, ResourceList()},
            {locbL, net.getPlace(locbL).getBaseQuants()},
            {locjL, ResourceList()},
            {locjR, ResourceList()}
        }));
        EXPECT_EQ(ALL_COSTS.getNumJunctions(net), 2);
        EXPECT_EQ(
            ALL_COSTS.getTrackLengthMap(net), 
            ((std::map<size_t, Dist>){
                {1, daR+daR + daL+daL + dbR+dbR + dbL+dbL}, 
                {2, dJ + dJ}
            })
        );
        EXPECT_EQ(ALL_COSTS.getMaxLength(net), daR+daR + daL+daL + dJ+dJ);
        EXPECT_EQ(ALL_COSTS.getNumRoutes(net), 2);
        EXPECT_DOUBLE_EQ(ALL_COSTS.getMaxCarryTime(net), 6*(daR+dJ+daL).toDouble());
        EXPECT_DOUBLE_EQ(ALL_COSTS.getTotalCarryTime(net), 6*(daR+dJ+daL).toDouble() + 5*(dbR+dJ+dbL).toDouble());
        EXPECT_EQ(ALL_COSTS.getMaxPeakCapacity(net), 4);
        EXPECT_EQ(ALL_COSTS.getTotalPeakCapacity(net), 8);

        EXPECT_DOUBLE_EQ(ALL_COSTS(net), 
            ALL_COSTS.weights[CostFunct::Metric::NumJunctions]*2 + 
            ALL_COSTS.weights[CostFunct::Metric::BaseTrackLength]*(daR+daR + daL+daL + dbR+dbR + dbL+dbL + dJ+dJ).toDouble() + 
            ALL_COSTS.weights[CostFunct::Metric::SharedTrackLength]*(dJ + dJ).toDouble() + 
            ALL_COSTS.weights[CostFunct::Metric::MaxLength]*(daR+daR + daL+daL + dJ+dJ).toDouble() + 
            ALL_COSTS.weights[CostFunct::Metric::NumRoutes]*2 + 
            ALL_COSTS.weights[CostFunct::Metric::MaxCarryTime]*6*(daR + daL + dJ).toDouble() + 
            ALL_COSTS.weights[CostFunct::Metric::TotalCarryTime]*(6*(daR + dJ + daL).toDouble() + 5*(dbR + dJ + dbL).toDouble()) + 
            ALL_COSTS.weights[CostFunct::Metric::MaxPeakCapacity]*4 + 
            ALL_COSTS.weights[CostFunct::Metric::TotalPeakCapacity]*8
        );
    }
}