/*
Brute Force Class definition
Author: Ethan Worth, Andrew Bergman, Mason Paladino, Nozomu Ohno
11/19/22

Derived solver for brute force
*/

#ifndef BRUTE_FORCE_H
#define BRUTE_FORCE_H

/////////////////
// Includes
/////////////////

#include "../Solver.h"

/////////////////
// Solver Class
/////////////////

class BruteForce : Solver {
    // solver
    Network solve();
};

#endif