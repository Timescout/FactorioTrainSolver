/*
Location data structure declaration
Author: Ethan Worth, Andrew Bergman, Mason Paladino, Nozomu Ohno
11/19/22

This file declares data structures and functions that handle location
in the coordinate plane. 
*/

#ifndef LOCATION_H
#define LOCATION_H

/////////////////
// Includes
/////////////////

#include <stdint.h>

/////////////////
// Definitions
/////////////////

// the type that we use to store coordinate values in our system
typedef int32_t Coord; // smaller gives annoying warnings.

/////////////////
// Data structures
/////////////////

// Stores a distance between two Locations
// Must be strictly positive
struct Dist {
    Coord rat_, irrat_;

    // addition operators
    // operates on Dist rat and irrat component wise ie. Dist1 + Dist2 = (rat1 + rat2, irrat1 + irrat2)
    Dist operator+(const Dist& other) const;
    Dist operator-(const Dist& other) const;
    Dist operator+=(const Dist& other);

    double toDouble() const; 
    
    // comparison operators
    // returns based on the actual length of the distance
    bool operator==(const Dist& other) const;
    bool operator>(const Dist& other) const;
    bool operator<(const Dist& other) const;
    bool operator>=(const Dist& other) const;
    bool operator<=(const Dist& other) const;
};

// using struct per ISO C++ guidelines: https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c2-use-class-if-the-class-has-an-invariant-use-struct-if-the-data-members-can-vary-independently
// Stores a location in the cartesian plane.
struct Location {
    Coord x, y;

    // constructor
    Location() : x(0x7fffffff), y(0x7fffffff) {}
    Location(Coord _x, Coord _y) : x(_x), y(_y) {}

    // operator overloads
    Location operator+(const Location& loc) const;
    Location operator-(const Location& loc) const;
    bool operator==(const Location& loc) const;

    bool operator<(const Location& other) const;
    bool operator>(const Location& other) const;
    bool operator<=(const Location& other) const;
    bool operator>=(const Location& other) const;
    
    // dist function (friend)
    friend Dist dist(const Location& l1, const Location& l2);
};

// Struct used to hash Locations.
// used in std::unordered_map. do std::unordered_map<Location, valuetype, LocationHasher>()
struct LocationHasher
{
    size_t operator()(const Location& key) const noexcept; // used to hash a Location
};

// Comparison struct used for Comparing two Locations.
// used in std::map. do std::map<Location, valuetype, LocationCompare>()
struct LocationCompare
{
    bool operator()(const Location& a, const Location& b) const noexcept; // used to compare two locations
};

#endif