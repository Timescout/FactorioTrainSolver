/*
Junction finder Function tests.
Author: Andrew Bergman
12/11/22

tests for the JunctionFunction, which finds good junciton points for a network.
*/

/////////////////////////////////
// includes
/////////////////////////////////

#include <gtest/gtest.h>
#include "../headers/solutions/Solver.h"
#include "TestSetup.h"
#include "../headers/solutions/CanonicalExamples.h"

/////////////////////////////////
// Test namespace
/////////////////////////////////

namespace JuncitonFunctionTest
{
    /////////////////////////////////
    // Constants and tools
    /////////////////////////////////

    // I need a solver that issn't an abstract base class, and so I make my own one that is very basic.
    class basicSolver : public Solver
    {
    public:
        basicSolver(Network net) :
        Solver(net, ALL_COSTS, Constraints())
        {}
        Network solve() { return Network(); }
    };

    const Network colinearVert(std::vector<Factory>({
        Factory(0, 1, ResourceList({{Resource::Copper, 1}})),
        Factory(0, 5, ResourceList({{Resource::Copper, 1}}))
    }));
    // good junction points are: (2,3), (-2,3)

    const Network colinearHor(std::vector<Factory>({
        Factory(1, 0, ResourceList({{Resource::Copper, 1}})),
        Factory(5, 0, ResourceList({{Resource::Copper, 1}}))
    }));
    // good junction points are: (3,2), (3,-2)

    const Network colinearRightDiag(std::vector<Factory>({
        Factory(0, 0, ResourceList({{Resource::Copper, 1}})),
        Factory(5, 5, ResourceList({{Resource::Copper, 1}}))
    }));
    // good junction points are: (5,0), (0,5)

    const Network colinearLeftDiag(std::vector<Factory>({
        Factory(0, 0, ResourceList({{Resource::Copper, 1}})),
        Factory(-5, 5, ResourceList({{Resource::Copper, 1}}))
    }));
    // good junction points are(0,5), (-5,0)

    const Network colinearOffParityVert(std::vector<Factory>({
        Factory(0, 0, ResourceList({{Resource::Copper, 1}})),
        Factory(0, 5, ResourceList({{Resource::Copper, 1}}))
    }));
    // no good junction points

    const Network colinearOffParityHor(std::vector<Factory>({
        Factory(0, 0, ResourceList({{Resource::Copper, 1}})),
        Factory(5, 0, ResourceList({{Resource::Copper, 1}}))
    }));
    // no good junction points

    const Network firstOctant(std::vector<Factory>({
        Factory(1, 1, ResourceList({{Resource::Copper, 1}})),
        Factory(9, 5, ResourceList({{Resource::Copper, 1}}))
    }));
    // good junction points: (9,1), (7,7), (5,5), (5,1), (3,-1), (1,5)

    const Network secondOctant(std::vector<Factory>({
        Factory(1, 1, ResourceList({{Resource::Copper, 1}})),
        Factory(3, 7, ResourceList({{Resource::Copper, 1}}))
    }));
    // good junction points: (5,5), (3,3), (3,1), (1,7), (1,5), (-1,3)

    const Network thirdOctant(std::vector<Factory>({
        Factory(0, 0, ResourceList({{Resource::Copper, 1}})),
        Factory(-6, 10, ResourceList({{Resource::Copper, 1}}))
    }));
    // good junction points: (2,2), (0,10), (0,4), (-6,6), (-6,0), (-8,8)

    const Network fourthOctant(std::vector<Factory>({
        Factory(0, 0, ResourceList({{Resource::Copper, 1}})),
        Factory(-13, 7, ResourceList({{Resource::Copper, 1}}))
    }));
    // good junction points: (0,7), (-3,-3), (-6,0), (-7,7), (-10,10), (-13,0)

    const Network doubleMul(std::vector<Factory>({
        Factory(0, 0, ResourceList({{Resource::Copper, 1}})),
        Factory(6, 6, ResourceList({{Resource::Copper, 1}})),
        Factory(6, -6, ResourceList({{Resource::Copper, 1}}))
    }));
    // good junction points: (6,0) 2x, (12,0) 1x, (0,6) 1x, (0,-6) 1x


    /////////////////////////////////
    // Tests
    /////////////////////////////////

    TEST(JunctionTest, ColinearVertical)
    {
        basicSolver solve(colinearVert);
        std::vector<std::pair<Location, int>> junctions = solve.junctionFunction(colinearVert.getNumFactories()*12); // make sure that any and all possible locations are included.
        EXPECT_EQ(junctions[0].first, Location(2,3));
        EXPECT_EQ(junctions[1].first, Location(-2,3));
        EXPECT_EQ(junctions[0].second, 1); // check the multiplicity of each location
        EXPECT_EQ(junctions[1].second, 1);
        for (int i = 2; i < junctions.size(); i++)
        {
            EXPECT_EQ(junctions[i].first, Location()); // everywhere else in the vector should be a default location.
        }
    }

    TEST(JunctionTest, ColinearHorizontal)
    {
        basicSolver solve(colinearHor);
        std::vector<std::pair<Location, int>> junctions = solve.junctionFunction(colinearHor.getNumFactories()*12); // make sure that any and all possible locations are included.
        EXPECT_EQ(junctions[0].first, Location(3,2));
        EXPECT_EQ(junctions[1].first, Location(3,-2));
        EXPECT_EQ(junctions[0].second, 1); // check the multiplicity of each location
        EXPECT_EQ(junctions[1].second, 1);
        for (int i = 2; i < junctions.size(); i++)
        {
            EXPECT_EQ(junctions[i].first, Location()); // everywhere else in the vector should be a default location.
        }
    }

    TEST(JunctionTest, ColinearRightDiagonal)
    {        
        basicSolver solve(colinearRightDiag);
        std::vector<std::pair<Location, int>> junctions = solve.junctionFunction(colinearRightDiag.getNumFactories()*12); // make sure that any and all possible locations are included.
        EXPECT_EQ(junctions[0].first, Location(5,0));
        EXPECT_EQ(junctions[1].first, Location(0,5));
        EXPECT_EQ(junctions[0].second, 1); // check the multiplicity of each location
        EXPECT_EQ(junctions[1].second, 1);
        for (int i = 2; i < junctions.size(); i++)
        {
            EXPECT_EQ(junctions[i].first, Location()); // everywhere else in the vector should be a default location.
        }
    }

    TEST(JunctionTest, ColinearLeftDiagonal)
    {
        basicSolver solve(colinearLeftDiag);
        std::vector<std::pair<Location, int>> junctions = solve.junctionFunction(colinearLeftDiag.getNumFactories()*12); // make sure that any and all possible locations are included.
        EXPECT_EQ(junctions[0].first, Location(0,5));
        EXPECT_EQ(junctions[1].first, Location(-5,0));
        EXPECT_EQ(junctions[0].second, 1); // check the multiplicity of each location
        EXPECT_EQ(junctions[1].second, 1);
        for (int i = 2; i < junctions.size(); i++)
        {
            EXPECT_EQ(junctions[i].first, Location()); // everywhere else in the vector should be a default location.
        }
    }

    TEST(JunctionTest, ColinearVertOffParity)
    {
        basicSolver solve(colinearOffParityVert);
        std::vector<std::pair<Location, int>> junctions = solve.junctionFunction(colinearOffParityVert.getNumFactories()*12); // make sure that any and all possible locations are included.
        for (int i = 0; i < junctions.size(); i++)
        {
            EXPECT_EQ(junctions[i].first, Location()); // everywhere in the vector should be a default location.
        }
    }

    TEST(JunctionTest, ColinearHorOffParity)
    {
        basicSolver solve(colinearOffParityHor);
        std::vector<std::pair<Location, int>> junctions = solve.junctionFunction(colinearOffParityHor.getNumFactories()*12); // make sure that any and all possible locations are included.
        for (int i = 0; i < junctions.size(); i++)
        {
            EXPECT_EQ(junctions[i].first, Location()); // everywhere in the vector should be a default location.
        }
    }

    TEST(JunctionTest, FirstOctant)
    {
        basicSolver solve(firstOctant);
        std::vector<std::pair<Location, int>> junctions = solve.junctionFunction(firstOctant.getNumFactories()*12); // make sure that any and all possible locations are included.
        // the junctions we expect are: (9,1), (7,7), (5,5), (5,1), (3,-1), (1,5)
        EXPECT_EQ(junctions[0].first, Location(9,1)); // check the right locations were found
        EXPECT_EQ(junctions[1].first, Location(7,7));
        EXPECT_EQ(junctions[2].first, Location(5,5));
        EXPECT_EQ(junctions[3].first, Location(5,1));
        EXPECT_EQ(junctions[4].first, Location(3,-1));
        EXPECT_EQ(junctions[5].first, Location(1,5));
        EXPECT_EQ(junctions[0].second, 1); // check the multiplicity of each location
        EXPECT_EQ(junctions[1].second, 1);
        EXPECT_EQ(junctions[2].second, 1);
        EXPECT_EQ(junctions[3].second, 1);
        EXPECT_EQ(junctions[4].second, 1);
        EXPECT_EQ(junctions[5].second, 1);
        for (int i = 6; i < junctions.size(); i++)
        {
            EXPECT_EQ(junctions[i].first, Location()); // everywhere in the vector should be a default location.
        }
    }

    TEST(JunctionTest, SecondOctant)
    {
        basicSolver solve(secondOctant);
        std::vector<std::pair<Location, int>> junctions = solve.junctionFunction(firstOctant.getNumFactories()*12); // make sure that any and all possible locations are included.
        // the junctions we expect are: (5,5), (3,3), (3,1), (1,7), (1,5), (-1,3)
        EXPECT_EQ(junctions[0].first, Location(5,5));
        EXPECT_EQ(junctions[1].first, Location(3,3));
        EXPECT_EQ(junctions[2].first, Location(3,1));
        EXPECT_EQ(junctions[3].first, Location(1,7));
        EXPECT_EQ(junctions[4].first, Location(1,5));
        EXPECT_EQ(junctions[5].first, Location(-1,3));
        EXPECT_EQ(junctions[0].second, 1); // check the multiplicity of each location
        EXPECT_EQ(junctions[1].second, 1);
        EXPECT_EQ(junctions[2].second, 1);
        EXPECT_EQ(junctions[3].second, 1);
        EXPECT_EQ(junctions[4].second, 1);
        EXPECT_EQ(junctions[5].second, 1);
        for (int i = 6; i < junctions.size(); i++)
        {
            EXPECT_EQ(junctions[i].first, Location()); // everywhere in the vector should be a default location.
        }
    }

    TEST(JunctionTest, ThirdOctant)
    {
        basicSolver solve(thirdOctant);
        std::vector<std::pair<Location, int>> junctions = solve.junctionFunction(firstOctant.getNumFactories()*12); // make sure that any and all possible locations are included.
        // the junctions we expect are: (2,2), (0,10), (0,4), (-6,6), (-6,0), (-8,8)
        EXPECT_EQ(junctions[0].first, Location(2,2));
        EXPECT_EQ(junctions[1].first, Location(0,10));
        EXPECT_EQ(junctions[2].first, Location(0,4));
        EXPECT_EQ(junctions[3].first, Location(-6,6));
        EXPECT_EQ(junctions[4].first, Location(-6,0));
        EXPECT_EQ(junctions[5].first, Location(-8,8));
        EXPECT_EQ(junctions[0].second, 1); // check the multiplicity of each location
        EXPECT_EQ(junctions[1].second, 1);
        EXPECT_EQ(junctions[2].second, 1);
        EXPECT_EQ(junctions[3].second, 1);
        EXPECT_EQ(junctions[4].second, 1);
        EXPECT_EQ(junctions[5].second, 1);
        for (int i = 6; i < junctions.size(); i++)
        {
            EXPECT_EQ(junctions[i].first, Location()); // everywhere in the vector should be a default location.
        }
    }

    TEST(JunctionTest, FourthOctant)
    {
        basicSolver solve(fourthOctant);
        std::vector<std::pair<Location, int>> junctions = solve.junctionFunction(firstOctant.getNumFactories()*12); // make sure that any and all possible locations are included.
        // the junctions we expect are: (0,7), (-3,-3), (-6,0), (-7,7), (-10,10), (-13,0)
        EXPECT_EQ(junctions[0].first, Location(0,7));
        EXPECT_EQ(junctions[1].first, Location(-3,-3));
        EXPECT_EQ(junctions[2].first, Location(-6,0));
        EXPECT_EQ(junctions[3].first, Location(-7,7));
        EXPECT_EQ(junctions[4].first, Location(-10,10));
        EXPECT_EQ(junctions[5].first, Location(-13,0));
        EXPECT_EQ(junctions[0].second, 1); // check the multiplicity of each location
        EXPECT_EQ(junctions[1].second, 1);
        EXPECT_EQ(junctions[2].second, 1);
        EXPECT_EQ(junctions[3].second, 1);
        EXPECT_EQ(junctions[4].second, 1);
        EXPECT_EQ(junctions[5].second, 1);
        for (int i = 6; i < junctions.size(); i++)
        {
            EXPECT_EQ(junctions[i].first, Location()); // everywhere in the vector should be a default location.
        }
    }

    TEST(JunctionTest, DoubleMUltiplicity)
    {
        basicSolver solve(doubleMul);
        std::vector<std::pair<Location, int>> junctions = solve.junctionFunction(firstOctant.getNumFactories()*12); // make sure that any and all possible locations are included.
        // the junctions we expect are: (6,0) 2x, (12,0) 1x, (0,6) 1x, (0,-6) 1x
        EXPECT_EQ(junctions[0].first, Location(6,0));
        EXPECT_EQ(junctions[1].first, Location(12,0));
        EXPECT_EQ(junctions[2].first, Location(0,6));
        EXPECT_EQ(junctions[3].first, Location(0,-6));
        EXPECT_EQ(junctions[0].second, 2); // check the multiplicity of each location
        EXPECT_EQ(junctions[1].second, 1);
        EXPECT_EQ(junctions[2].second, 1);
        EXPECT_EQ(junctions[3].second, 1);

        for (int i = 6; i < junctions.size(); i++)
        {
            EXPECT_EQ(junctions[i].first, Location()); // everywhere in the vector should be a default location.
        }
    }

} // namespace 