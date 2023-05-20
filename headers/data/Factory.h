/*
Factory Class declaration
Author: Ethan Worth, Andrew Bergman, Mason Paladino, Nozomu Ohno
11/19/22

The factory class holds information about factories in the network.
Factories have 2 sets associated with them: 
A set of resources that it demands (represented by negative values) 
and supplies (positive values).
And a set of allocated resources that tracks what resources have 
been supplied and consumed by the network.
*/

#ifndef FACTORY_H
#define FACTORY_H

/////////////////
// Includes
/////////////////

#include <map>
#include "Location.h"
#include "ResourceList.h"

/////////////////
// Factory Class
/////////////////

// Factory class
// This class models a Factory in our system.
// A factory is a production hub that produces goods
// and consumes materials in order to accomplish that.
// Thus it has a set of resources that it needs and
// a set that it produces. It also has a location in space.
class Factory {
private:
    const Location loc_;
    const ResourceList base_quants_; // neg Quant means net demand, pos Quant means net produce
    ResourceList unallocated_;

public:
    // constructor
    Factory();
    Factory(Coord x, Coord y, ResourceList netResources = ResourceList()); // Andrew Likes to use this one. 
    Factory(Location l, ResourceList rl = ResourceList());
    // factory method
    static Factory makeJunction(Location l);
    static Factory makeJunction(Coord x, Coord y);

    // getters
    Location getLoc() const;
    ResourceList getBaseQuants() const;
    ResourceList getUnallocated() const;
    ResourceList getAllocated() const;
    
    // allocation manager (must be executed in coordination with Route)
    // positive values in rl represent a quantity that is pickedup, while
    // a negative value represents droped off (think of this from the
    // perspective of the train)
    bool allocate(ResourceList rl);
    // deallocate is just allocation with a negated rl... right???
    bool deallocate(ResourceList rl);

    // resets Allocated resources
    // basically just sets unallocated_ to base_quants_
    void resetAllocated();
};

#endif