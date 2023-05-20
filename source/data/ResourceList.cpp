/*
ResourceList data structure definitions
Author: Ethan Worth, Andrew Bergman, Mason Paladino, Nozomu Ohno
11/19/22

Definitions for ResourcesLists
*/

/////////////////
// Includes
/////////////////

#include "../../headers/data/ResourceList.h"

/////////////////
// Resource Functions
/////////////////

// https://stackoverflow.com/a/2571531
// prefix operator
Resource& operator++(Resource& r) {
    return r = static_cast<Resource>((r+1) % (COUNT+1));
}
// https://stackoverflow.com/a/2571531
// postfix operator
Resource operator++(Resource& r, int) {
    Resource orig = r;
    r = static_cast<Resource>((r+1) % (COUNT+1));
    return orig;
}
// https://stackoverflow.com/a/2571531
Resource& operator--(Resource& r) {
    if(r == 0) {
        return r = Resource::COUNT;
    }
    return r = static_cast<Resource>(r-1);
}
// https://stackoverflow.com/a/2571531
Resource operator--(Resource& r, int) {
    Resource orig = r;
    if(r == 0) {
        r = Resource::COUNT;
    } else {
        r = static_cast<Resource>(r-1);
    }
    return orig;
}

///////////////////////////////////////////////////////////////////////////////
// ResourceList Functions
///////////////////////////////////////////////////////////////////////////////

ResourceList::ResourceList() {
    for(ResourceList::iterator i = begin(); i != end(); i++)
        *i = 0;
}
// assign resource list from a map, which can be created with an initializer_list
ResourceList::ResourceList(std::map<Resource, Quant> rs) {
    for(int i = 0; Resource(i) != Resource::COUNT; i++ ) {
        operator[](i) = rs.find(Resource(i))->second;
    }
}

bool ResourceList::isEmpty() const {
    for(ResourceList::const_iterator i = begin(); i != end(); i++)
        if(*i != 0)
            return false;
    return true;
}

ResourceList ResourceList::operator+(const ResourceList& rl) const {
    ResourceList sum;
    for(Resource r = Resource(0); r != Resource::COUNT; r++)
        sum[r] = operator[](r) + rl[r];
    return sum;
}

ResourceList ResourceList::operator-(const ResourceList& rl) const {
    ResourceList difference;
    for(Resource r = Resource(0); r != Resource::COUNT; r++)
        difference[r] = operator[](r) - rl[r];
    return difference;
}

ResourceList& ResourceList::operator+=(const ResourceList& other)
{
    *this = *this + other;
    return *this;
}

ResourceList& ResourceList::operator-=(const ResourceList& other)
{
    *this = *this - other;
    return *this;
}

bool ResourceList::allPositive() const
{
    for (ResourceList::const_iterator i = cbegin(); i != end(); i++)
    {
        if (*i < 0)
        {
            return false;
        }
    }
    return true;
}

std::list<Resource> ResourceList::getNegative() const
{
    std::list<Resource> negatives;
    for (Resource r = Resource(0); r != Resource::COUNT; r++)
    {
        if (operator[](r) < 0)
        {
            negatives.push_back(r);
        }
    }
    return negatives;
}

std::list<Resource> ResourceList::getPositive() const
{
    std::list<Resource> positives;
    for (Resource r = Resource(0); r != Resource::COUNT; r++)
    {
        if (operator[](r) > 0)
        {
            positives.push_back(r);
        }
    }
    return positives;
}
// get a RL of the resources are that are negative in this list
//   all other Quants set to 0
ResourceList ResourceList::getNegativeList() const {
    ResourceList neg_rl;
    for(const Resource& r : getNegative())
        neg_rl[r] = operator[](r);
    return neg_rl;
}
// get a RL of the resources are that are positive in this list
//   all other Quants set to 0
ResourceList ResourceList::getPositiveList() const {
    ResourceList post_rl;
    for(const Resource& r : getPositive())
        post_rl[r] = operator[](r);
    return post_rl;
}

bool ResourceList::operator==(const ResourceList& other) const
{
    for(int i = 0; i < size(); i++)
    {
        if(operator[](i) != other[i])
        {
            return false;
        }
    }
    return true;
}