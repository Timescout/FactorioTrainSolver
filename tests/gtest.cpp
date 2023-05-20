#include <gtest/gtest.h>
#include "TestSetup.h"

// #include "ResourceTest.h"
// #include "LocationTest.h"
// #include "DistTest.h"
// #include "FactoryTest.h"
// #include "RouteTest.h"
// #include "NetworkTest.h"

// #include "ConstraintsTest.h"
// #include "CostFunctTest.h"

#include "JunctionFunctionTest.h"
// #include "CanonicalExamplesTest.h"
// #include "GreedyEdgeListTest.h"
// #include "GeneticTest.h"

#include "SolverAnalysis.h"

int main(int argc, char **argv) {
    setupTests();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}