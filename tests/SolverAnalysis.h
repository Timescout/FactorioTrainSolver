/*
Junction Function Analysis
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
#include "../headers/solutions/Solver.h"

#include<chrono>
#include<iostream>

/////////////////
// tests
/////////////////

//Test good network for solution constraints
//Check Twice
namespace SoverTest{
    class basicSolver : public Solver
    {
    public:
        basicSolver(Network net) :
        Solver(net, ALL_COSTS, Constraints())
        {}
        Network solve() { return Network(); }
    };

    TEST(SolverAnalysis, JunctionFunctionFactoryN){
        double avg = 0;
        for(int i = 0; i < 100; i++){
            auto start = std::chrono::high_resolution_clock::now();

            Network net = randomNetwork(0, 50, 100);
            basicSolver slv(net);
            EXPECT_TRUE(slv.junctionFunction(5).size() == 5);

            auto end = std::chrono::high_resolution_clock::now();
            avg += (end - start).count();
            std::cout << "Test " + std::to_string(i + 1) + ": " + std::to_string((end - start).count()) << std::endl;
        }
        avg = avg / 100;
        std::cout << "Average: " + std::to_string(avg);
    }   

    TEST(SolverAnalysis, JunctionFunctionFactory2N){
        double avg = 0;
        for(int i = 0; i < 100; i++){
            auto start = std::chrono::high_resolution_clock::now();

            Network net = randomNetwork(0, 100, 100);
            basicSolver slv(net);
            EXPECT_TRUE(slv.junctionFunction(5).size() == 5);

            auto end = std::chrono::high_resolution_clock::now();
            avg += (end - start).count();
            std::cout << "Test " + std::to_string(i + 1) + ": " + std::to_string((end - start).count()) << std::endl;
        }
        avg = avg / 100;
        std::cout << "Average: " + std::to_string(avg);
    }

    TEST(SolverAnalysis, JunctionFunctionGridN){
        double avg = 0;
        for(int i = 0; i < 100; i++){
            auto start = std::chrono::high_resolution_clock::now();

            Network net = randomNetwork(0, 50, 100);
            basicSolver slv(net);
            EXPECT_TRUE(slv.junctionFunction(5).size() == 5);

            auto end = std::chrono::high_resolution_clock::now();
            avg += (end - start).count();
            std::cout << "Test " + std::to_string(i + 1) + ": " + std::to_string((end - start).count()) << std::endl;
        }
        avg = avg / 100;
        std::cout << "Average: " + std::to_string(avg);
    }

    TEST(SolverAnalysis, JunctionFunctionGrid2N){
        double avg = 0;
        for(int i = 0; i < 100; i++){
            auto start = std::chrono::high_resolution_clock::now();

            Network net = randomNetwork(0, 50, 200);
            basicSolver slv(net);
            EXPECT_TRUE(slv.junctionFunction(5).size() == 5);

            auto end = std::chrono::high_resolution_clock::now();
            avg += (end - start).count();
            std::cout << "Test " + std::to_string(i + 1) + ": " + std::to_string((end - start).count()) << std::endl;
        }
        avg = avg / 100;
        std::cout << "Average: " + std::to_string(avg);
    }
}