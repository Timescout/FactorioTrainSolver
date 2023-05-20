/*
location data structure difinition
Author: Ethan Worth, Andrew Bergman, Mason Paladino, Nozomu Ohno
11/19/22

Definitions for Location Data structure.
*/

/////////////////
// Includes
/////////////////

#include "../../headers/data/Location.h"
#include "math.h"
#include <algorithm>
#include <map>
#define _USE_MATH_DEFINES

/////////////////
// Dist Overloads
/////////////////

// operator overloads
Dist Dist::operator+(const Dist& other) const {
    return (Dist){rat_+other.rat_, irrat_+other.irrat_};
}
Dist Dist::operator+=(const Dist& other) {
    return (*this) = (*this) + other;
}
Dist Dist::operator-(const Dist& other) const {
    Dist t = (Dist){rat_-other.rat_, irrat_-other.irrat_};
    // if resulting distance is negative, flip signs
    if(t.toDouble() < 0)
        t = (Dist){-t.rat_, -t.irrat_};
    return t;
}

double Dist::toDouble() const {
    return rat_ + irrat_*M_SQRT2;
}

bool Dist::operator==(const Dist& other) const {
    return rat_==other.rat_ && irrat_==other.irrat_;
}

bool Dist::operator>(const Dist& other) const
{
    return this->toDouble() > other.toDouble();
}

bool Dist::operator<(const Dist& other) const
{
    return this->toDouble() < other.toDouble();
}

bool Dist::operator>=(const Dist& other) const
{
    return operator==(other) || operator>(other);
}

bool Dist::operator<=(const Dist& other) const
{
    return operator==(other) || operator<(other);
}

/////////////////
// Location Overloads
/////////////////

// operator overloads
Location Location::operator+(const Location& loc) const {
    return Location(x+loc.x, y+loc.y);
}
Location Location::operator-(const Location& loc) const {
    return Location(x-loc.x, y-loc.y);
}
bool Location::operator==(const Location& loc) const {
    return x == loc.x && y == loc.y;
}

bool Location::operator<(const Location& other) const
{
    if (x != other.x) {return x < other.x;}
    else {return y < other.y;}
}

bool Location::operator>(const Location& other) const
{
    return other < *this;
}

bool Location::operator<=(const Location& other) const
{
    return *this < other or *this == other;
}

bool Location::operator>=(const Location& other) const
{
    return *this > other or *this == other;
}


/////////////////////////
// Location Hash Function
/////////////////////////

size_t LocationHasher::operator()(const Location& key) const noexcept
{
    return 17*key.x + 7919*key.y;
}

/////////////////////////////
// Location Compare Function
/////////////////////////////

bool LocationCompare::operator()(const Location& a, const Location& b) const noexcept
{
    return a < b;
}


/////////////////
// Location to Dist converter
/////////////////

// dist function (friend)
Dist dist(const Location& l1, const Location& l2) {
    // max, min
    Coord maxx = std::abs(l1.x-l2.x), minn = std::abs(l1.y-l2.y);
    if(maxx < minn)
    {
        // swap maxx and minn
        std::swap(maxx, minn);
    }
    return (Dist){maxx-minn, minn};
}