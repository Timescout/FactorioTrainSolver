/*
Solver Class definition
Author: Ethan Worth, Andrew Bergman, Mason Paladino, Nozomu Ohno
11/19/22

The Solver class will take constraints, cost, and a list of factories
then find the most optimal way to build tracks between the factories to
maximize efficiency and minimize cost
*/

#ifndef SOLVER_H
#define SOLVER_H

/////////////////
// Includes
/////////////////

#include "../data/Network.h"
#include "CostFunct.h"
#include "Constraints.h"
#include <vector>

/////////////////
// Solver Class
/////////////////

class Solver {
    protected:
        Network network;
        CostFunct cost;
        Constraints constraints;

    public:
        // constructor
        Solver(const Network& net, const CostFunct cos, const Constraints constr);

        // solver
        bool canSolve() const;
        virtual Network solve() = 0;
        
        // helper functions
        Cost getCost() const;
        bool passesConstraints() const;


        // generate junctions
        // This function generates junctions and gives them to you as a list of locations where junctions would probably be very good.
        // This function only considers junctions between Factories in the network, and will not create junctions points for junctions
        // that already exist within the network.
        // apprx 4 to 6 junctions will be added per factory pair, so about (6*n^2)/2 points. Points may be repeated so the actual number
        // may be less. 
        // numberJunctions is the number of junctions you want to return. essentially "give me the best n junctions". To include everything, 
        // set this to network.size()*6. Ask Andrew for why.
        // will not include locations already in the network.
        std::vector<std::pair<Location, int>> junctionFunction(size_t numberJunctions);

        // getter
        const Network& getNet() const;

};

#endif