/*
Resource Enum definition and ResourceList class definition
Author: Ethan Worth, Andrew Bergman, Mason Paladino, Nozomu Ohno
11/19/22

Enum for each resource type
Uses overloaded functions for ++ and -- 
*/

#ifndef RESOURCE_LIST_H
#define RESOURCE_LIST_H

/////////////////
// Includes
/////////////////

#include <array>
#include <vector>
#include <list>
#include <map>

/////////////////
// Custom sign tracker per: https://stackoverflow.com/a/4609795
/////////////////
typedef int8_t Sign;
const Sign NEG_S = -1, ZERO_S = 0, POS_S = 1;
// can't put this in .cpp because of templating
template <typename T>
Sign signof(T n) {
    return (T(0) < n) - (n < T(0));
}

/////////////////
// Resource enum
/////////////////

// type for keeping track of different resources
// to create a resource from an integer use:
// Resource a = Resource(0)
// for example
enum Resource { Copper, Steel, Iron, Stone, Uranium, Circuits, Fish, Wood, NuclearFuel, Wire, Engine, Gear, COUNT };

// not prefered, use ResourceList::iterator to iterate through a list of resources if possible

// https://stackoverflow.com/a/2571531
Resource& operator++(Resource& r);
Resource operator++(Resource& r, int);
// https://stackoverflow.com/a/2571531
Resource& operator--(Resource& r);
Resource operator--(Resource& r, int);

// type used for a number of resources. negative Quant represents a resource demand.
typedef int32_t Quant;

// TODO: document neg/pos bevhaior etc
// neg Quant = demand
// pos Quant = produce

// ResourceList class
// this stores a list of resources that can be accessed with [] and 
// provides information about what resources it contains.
class ResourceList : public std::array<Quant, Resource::COUNT> {
private:

public:

    // default constructor assigns all Quants to 0
    ResourceList();
    // assign resource list from a map, which can be created with an initializer_list
    ResourceList(std::map<Resource, Quant> rs);

    // is the list empty?
    // returns whether all quatities are 0
    bool isEmpty() const;

    // are all values positive
    // checks that all values in the list are positive
    bool allPositive() const;

    // get list of which resources are positive or negative in this list
    std::list<Resource> getNegative() const;
    std::list<Resource> getPositive() const;
    // get a RL of the resources are that are positive or negative in this list
    //   all other Quants set to 0
    ResourceList getNegativeList() const;
    ResourceList getPositiveList() const;

    // adds or subtracts two lists element wise
    ResourceList operator+(const ResourceList& rl) const;
    ResourceList operator-(const ResourceList& rl) const;

    ResourceList& operator+=(const ResourceList& rl);
    ResourceList& operator-=(const ResourceList& rl);

    // define == element wise; Two Resourcelists are
    // equal if each resource has the same quantity 
    // in each list.
    bool operator==(const ResourceList& other) const;

};

#endif