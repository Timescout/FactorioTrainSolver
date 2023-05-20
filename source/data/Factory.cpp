/*
Factory Class definition
Author: Ethan Worth, Andrew Bergman, Mason Paladino, Nozomu Ohno
11/19/22

Definitions for Factory class.
*/

/////////////////
// Includes
/////////////////

#include "../../headers/data/Factory.h"

/////////////////
// Functions
/////////////////

// Constructors
Factory::Factory() : 
loc_(0,0)
{
}

Factory::Factory(Coord x, Coord y, ResourceList netResources) :
base_quants_(netResources),
unallocated_(netResources),
loc_(Location(x,y))
{
}

Factory::Factory(Location location, ResourceList netResources) :
loc_(location), 
base_quants_(netResources),
unallocated_(netResources) 
{
}

// factory method
Factory Factory::makeJunction(Location l) {
    return Factory(l);
}

Factory Factory::makeJunction(Coord x, Coord y) {
    return Factory(x, y);
}

// getters
Location Factory::getLoc() const {
    return loc_;
}
ResourceList Factory::getBaseQuants() const {
    return base_quants_;
}
ResourceList Factory::getUnallocated() const {
    return unallocated_;
}
ResourceList Factory::getAllocated() const {
    // start with base quants
    ResourceList out = base_quants_;
    // subtract off allocated
    for(Resource r = static_cast<Resource>(0); r != Resource::COUNT; r++)
        out[r] -= unallocated_[r];
    // return result
    return out;
}

// Allocate and DeAllocate
bool Factory::allocate(ResourceList rl) {
    bool succ = true;
    Resource r;
    Sign base_s, rl_s;
    for(r = static_cast<Resource>(0); r != Resource::COUNT; r++) {
        base_s = signof(base_quants_[r]);
        rl_s = signof(rl[r]);

        // check that allocating sign (equals base sign OR zero)
        //  AND there is enough unallocated capacity
        if((rl_s == base_s || rl_s == ZERO_S) && abs(unallocated_[r]) >= abs(rl[r])) {
            // allocate
            unallocated_[r] -= rl[r]; 
        } else {
            // else fail
            succ = false;
            break;
        }
    }
    if(!succ) {
        r--;
        while(r != Resource::COUNT) {
            unallocated_[r] += rl[r];
            r--;
        }
    }
    return succ;
}
bool Factory::deallocate(ResourceList rl) {
    bool succ = true;
    Resource r;
    Sign base_s, rl_s;
    ResourceList allocated = getAllocated();
    for(r = static_cast<Resource>(0); r != Resource::COUNT; r++) {
        base_s = signof(base_quants_[r]);
        rl_s = signof(rl[r]);

        // check that allocating sign (equals zero or base sign not 0 and rl not base sign)
        //  AND there is enough allocated capacity
        if(((rl_s == ZERO_S) || (base_s != ZERO_S && rl_s == base_s)) && abs(allocated[r]) >= abs(rl[r])) {
            // deallocate
            unallocated_[r] += rl[r]; 
        } else {
            // else fail
            succ = false;
            break;
        }
    }
    if(!succ) {
        r--;
        while(r != Resource::COUNT) {
            unallocated_[r] -= rl[r];
            r--;
        }
    }
    return succ;
}
// resets Allocated resources
// basically just sets unallocated_ to base_quants_
void Factory::resetAllocated() {
    unallocated_ = base_quants_;
}