/*
Constraints declaration
Author: Ethan Worth, Andrew Bergman, Mason Paladino, Nozomu Ohno
11/19/22

This file defines structures used to determine if a network is 
valid. For example a network must have net positive resource
production for each resource.
*/

#ifndef CONSTRAINTS_H
#define CONSTRAINTS_H

/////////////////
// Includes
/////////////////

#include "../data/Network.h"

/////////////////
// Definitions
/////////////////

// Object that checks and enforces constraints on a Network
// There are two types of constraints, the first checks that
// a given question has an answer, or checks the question
// constraints. The second checks that an answer is valid,
// or verifies answer constraints
struct Constraints {
    // member variables are flags/parameters to configure constraints
    // bool allow_route_waste, satisfying_demand;

    // netResources always comes out to 0s

    // constructor (dependent on member variables)
    Constraints();

    // checks both question and answer constraints on 
    // a network, to only test either question or answer
    // use isValidSolution and isValidNetwork
    bool operator()(const Network& net) const;

    /////////////////
    // Solution constraints
    /////////////////

    // determine if a Network solution satisfies
    // all constraints on a solution
    bool isValidSolution(const Network& net) const;

    // Check that all routes leave all factories
    // a constraint that we have is that a route
    // cannot have the same factory twice in a row
    // during its path. this function checks that.
    bool checkTwice(const Network& net) const;

    // check that all factories are satisfied
    // essentially checks that products demands 
    // are met in every Factory
    bool checkSatisfied(const Network& net) const;

    /////////////////
    // Question constraints
    /////////////////

    // checks that a given network is valid,
    // for example a constraint is that the total sum
    // of all resources produced must be equal to or 
    // greater than the total consumed
    bool isValidNetwork(const Network& net) const;

    // check for net positive resource production
    // checks that every resource is produced enough
    // to satisfy the consumption demand for that
    // resource
    bool checkNetResources(const Network& net) const;


};

#endif