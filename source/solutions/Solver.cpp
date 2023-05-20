/*
Solver Class definitions
Author: Ethan Worth, Andrew Bergman, Mason Paladino, Nozomu Ohno
11/19/22

Definitions for the Solver class.
*/

/////////////////
// Includes
/////////////////
#include "../../headers/solutions/Solver.h"
#include <queue>
#include <algorithm>


/////////////////
// Functions
/////////////////
Solver::Solver(const Network& net, const CostFunct cos, const Constraints constr)
    : network(net), cost(cos), constraints(constr) {}

bool Solver::canSolve() const {
    return constraints.isValidNetwork(network);
}
Cost Solver::getCost() const {
    return 0;
    //return cost(network); //TODO: fix this when cost function is working properly.
}
bool Solver::passesConstraints() const {
    return constraints(network);
}


/////////////////////////////////////////
// Junction function
/////////////////////////////////////////

void inline increment(std::map<Location, int>& map, Location key)
{
    std::map<Location, int>::iterator iterator = map.find(key);
    if (iterator == map.end())
    {
        map.insert(std::pair<Location, int>(key, 1));
    }
    else
    {
        iterator->second++;
    }
}

std::vector<std::pair<Location, int>> Solver::junctionFunction(size_t numberJunctions)
{
    std::map<Location, int> locations;

    // iterate over each pair of factories in the network
    for (auto a = network.factCBegin(); a!= network.factCEnd(); a++)
    {
        // check that a is a factroy
        if (a->second.getBaseQuants() == ResourceList()) continue;
        auto b = a;
        b++;
        while(b != network.factCEnd())
        {
            if (b->second.getBaseQuants() == ResourceList()) continue;

            // grab and decouple the Locations of a and b
            Coord aX = a->first.x, aY = a->first.y;
            Coord bX = b->first.x, bY = b->first.y;
            Location octant = b->first - a->first;
            // calculate the intersections of the lines and add them to the locations vector, these are good points for junctions.

            if (octant.x != 0 and octant.y != 0) // check colinearity for streights
            {
                increment(locations, Location(aX,bY)); // Locations for the first hanan map
                increment(locations, Location(bX,aY));
            }
            // check parity of the Locations. If this value is 0, then the intersection of the diagonal lines will be on integer coordinates and a valid point in our space
            // we also check colinearity on diagonals
            if((aX+bX+aY+bY)%2 == 0 and ((abs(octant.x) - abs(octant.y)) != 0))
            {
                increment(locations, Location((aX+aY+bX-bY)/2, (aX+aY-bX+bY)/2)); // locations for the diagonal intersections
                increment(locations, Location((aX-aY+bX+bY)/2, (-aX+aY+bX+bY)/2));
            }
            // intersections of steight lines and diagonal lines. these only are valid if the points are in specific octants of each other
            short truth = (signof<Coord>(octant.x) == signof<Coord>(octant.y))*10 + (abs(octant.x) > abs(octant.y));
            switch (truth)
            {
            case 11: // first/fifth octant
                increment(locations, Location(bX+aY-bY, aY));
                increment(locations, Location(aX+bY-aY, bY));
                break;
            case 10: // second/sixth octant
                increment(locations, Location(aX, bY+aX-bX));
                increment(locations, Location(bX, aY+bX-aX));
                break;
            case 0: // third/seventh octant
                increment(locations, Location(aX, bY-aX+bX));
                increment(locations, Location(bX, aY-bX+aX));
                break;
            case 1: // fourth/eighth octant
                increment(locations, Location(bX-aY+bY, aY));
                increment(locations, Location(aX-bY+aY, bY));
                break;
            default:
                throw std::out_of_range("Invalid Octant in JunctionFunciton, Something evil this way comes.");
                break;
            } // switch
            b++;
        }// while
    }// for

    // so now all of the junction points have been found and their multiplicity counted. Now to sort them and prep them for output.
    std::vector<std::pair<Location, int>> output;
    output.reserve(locations.size());
    for (auto i = locations.begin(); i != locations.end(); i++)
    {
        if (!network.hasPlace(i->first)) output.push_back(*i); // I didn't put squiggles just for you Ethan.
    }
    // sort the output vector, first by multiplicity then by location.
    // cite sources: https://stackoverflow.com/questions/279854/how-do-i-sort-a-vector-of-pairs-based-on-the-second-element-of-the-pair
    std::sort(output.begin(), output.end(), [](const std::pair<Location, int> &left, const std::pair<Location, int> &right) 
    {
        if (left.second != right.second) return left.second > right.second;
        else return left.first > right.first;
    });

    output.resize(numberJunctions);
    return output;
} // junctionFunction

const Network& Solver::getNet() const {
    return network;
}

