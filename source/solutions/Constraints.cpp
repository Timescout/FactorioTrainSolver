/*
Constraints definitions
Author: Ethan Worth, Andrew Bergman, Mason Paladino, Nozomu Ohno
11/19/22

Definitions for Constraints
*/

/////////////////
// Includes
/////////////////
#include "../../headers/solutions/Constraints.h"

/////////////////
// Constructor
/////////////////

Constraints::Constraints(){}

////////////////
// Functions
/////////////////

bool Constraints::operator()(const Network& net) const
{
    return 
    (
        isValidNetwork(net) &&
        isValidSolution(net)
    );
}


bool Constraints::isValidSolution(const Network& net) const 
{
    return (
        checkTwice(net) &&
        checkSatisfied(net)
    );
}

// O(r * f_r)
bool Constraints::checkTwice(const Network& net) const
{
    // iterate over each route in the network
    for (std::vector<Route>::const_iterator route = net.routeCBegin(); route != net.routeCEnd(); route++)
    {
        // check that the first and last factory of the route are different
        if ((route->cbegin()->first) == ((--route->cend())->first))
            return false;
        
        // iterate over each factory in the route
        for(PairList<FactoryKey, ResourceList>::const_iterator factory = ++route->cbegin(); factory != route->cend(); factory++)
            if (factory->first == std::prev(factory)->first)
                return false;
    }
    // if no bad routes are found, return true
    return true;
    // TODO: Reimplement this Write tests for this
}

// O(f * R)
bool Constraints::checkSatisfied(const Network& net) const
{
    // iterate over all factories in the network and check that they are satisfied
    for (std::map<FactoryKey, Factory, LocationCompare>::const_iterator fact_it = net.factCBegin(); fact_it != net.factCEnd(); fact_it++)
    {
        ResourceList unallocated = fact_it->second.getUnallocated();
        for (Resource resource = static_cast<Resource>(0); resource < Resource::COUNT; resource++)
            if (unallocated[resource] < 0)
                return false;
    }
    return true;
    // TODO: write tests for this
}

bool Constraints::isValidNetwork(const Network& net) const
{
    return (
        checkNetResources(net)
    );
}

// O(f * R)
bool Constraints::checkNetResources(const Network& net) const
{
    ResourceList runningTotal;
    // iterate over all factories in the network and add the supply and demand for each resource to the running total
    for (std::map<FactoryKey, Factory, LocationCompare>::const_iterator fact_it = net.factCBegin(); fact_it != net.factCEnd(); fact_it++)
    {
        ResourceList baseProduction = fact_it->second.getBaseQuants();
        for (Resource resource = static_cast<Resource>(0); resource < Resource::COUNT; resource++)
        {
            runningTotal[resource] += baseProduction[resource];
        }
    }
    // check for negative net quant
    for (Resource resource = static_cast<Resource>(0); resource < Resource::COUNT; resource++)
        if (runningTotal[resource] < 0)
            return false;
    // else return true
    return true;
}