/*
Route Class definition
Author: Ethan Worth, Andrew Bergman, Mason Paladino, Nozomu Ohno 11/19/22
First Draft: Ethan Worth 11/??/22
Expanded: Ethan Worth 11/30/22

Route is a class that stores a series of edges connecting factories
We can also request the track length of a route or the net resources
taken across a route
*/

#ifndef ROUTE_H
#define ROUTE_H

/////////////////
// Includes
/////////////////

#include "ResourceList.h"
#include "Location.h"
#include "PairList.h"
#include <map>

/////////////////
// Definitions
/////////////////

typedef Location FactoryKey;

/////////////////
// Route Class
/////////////////

// Route Class
// This class stores information about a route in a network. It stores a list of factory indexes and ResourceLists. 
// The factory index is the index of some factory in some array structure somewhere outside the route. Thus routes
// are meaningless without this outside list of factories existing. The Network class will handle allocating/deallocating
// resourceLists in factories.
// The ResourceList at each stop can be interpreted as the "command" of the route at that stop. 
class Route {
private:
    PairList<FactoryKey, ResourceList> stops_; // list allocated to factory
public:
    // constuctor
    // must specify at least 2 initial factory visits
    //   if init_facts.size() is less than 2, throw invalid_argument
    Route(PairList<FactoryKey, ResourceList> init_facts);

    // construct a Route with two stops, optional resourceLists
    Route(FactoryKey from, FactoryKey to, ResourceList fromResourceList = ResourceList(), ResourceList toResourceList = ResourceList());

    // const_iterator getters for the internal container of stops
    //   iterator dereferences to std::pair<FactoryKey, ResourceList>
    //   so cbegin()->first gets key of first factory
    //   and cend()-second gets RL of first factory
    PairList<FactoryKey, ResourceList>::const_iterator cbegin() const;
    PairList<FactoryKey, ResourceList>::const_iterator cend() const;

    // adds the stop to stops_
    // warning: does not check if the given factory can produce or accept the given resourceList.
    // Reccommended to use Network.addStop()
    bool addStop(FactoryKey factoryIndex, ResourceList allocated = ResourceList()); // puts the stop at the end
    bool addStop(FactoryKey factoryIndex, size_t index, ResourceList allocated = ResourceList()); // puts stop at index, if possible else puts at end

    // Removes the first instance of the factoryIndex after index past (including Route[past])
    // warning: does not adjust factory allocated resources, reccomended to use Network.dropStop()
    // returns true if the given factoryIndex is found and removed, false otherwise.
    bool dropStop(FactoryKey factoryIndex, int past = 0);

    // returns the key of the factory at the given stop index in the route
    // Use to iterate over all stops in a Route
    FactoryKey& operator[](size_t index);

    // Get the resourceList of a stop.
    const ResourceList getResources(size_t stop) const;

    // get the quantity of a Resource at a stop.
    const Quant getResourceAmount(size_t stop, Resource resource) const;

    // add up resources dropped off and picked up all stops to get net resourcelist
    // to fit constraints, the resulting RL should have no negative quants
    ResourceList getNetResources() const;

    // returns how many stops this route has.
    size_t size() const;

    // checks if this route has the given factory
    // if the factory is found returns the first
    // stop with that factory
    // returns -1 if the given factory is not found
    const int findStop(FactoryKey index) const;

    // sets the given stop to have the given ResouceList
    void setResourceList(size_t stop, ResourceList updatedList);

    // sets a resource in a given stop to have the given quantity
    void setResource(size_t stop, Resource resource, Quant quantity);

    // compares two Routes
    // two routes are equal if their stops match and the ResourceList
    // at each stop also matches.
    const bool operator==(const Route& other) const;

    /////////////////////////////////////////////////////////////////
    // Information functions
    /////////////////////////////////////////////////////////////////

    // Calculates which resources are effectively "carried over" from one cycle of the route to the next
    ResourceList getCarryover() const;

    // get total length of track this route uses
    Dist getLength() const; 

    // gets the number of factories that this route visits
    size_t getNumStops() const; 
    
    // CarryTime = Dist*Quant
    // Measures the total distance resources are carried in the route
    double getCarryTime() const;

    // Returns the maximum number of resources carried by the route at any given time
    //  this effectively represents the necesasry capacity of the train
    Quant getPeakCapacity() const;

    // reverse order of stops, without changing start
    void reverse();
    // shift start to the nth element in the stops_ list
    //   default: shift by 1
    void rotate(size_t n = 1);
};

#endif