/*
Route data structure definitions
Author: Ethan Worth, Andrew Bergman, Mason Paladino, Nozomu Ohno 11/19/22
First Draft: Ethan Worth 11/??/22
Expanded: Ethan Worth 11/30/22

Definitions for Route data structure
*/

/////////////////
// Includes
/////////////////

#include "../../headers/data/Route.h"
#include <exception>
#include <algorithm>

/////////////////
// Constructor
/////////////////

Route::Route(PairList<FactoryKey, ResourceList> init_facts) :
stops_(init_facts)
{
    // check for enough facts
    if(init_facts.size() < 2)
        throw std::invalid_argument("Route must be initialized with at least 2 factories");
}

Route::Route(FactoryKey from, FactoryKey to, ResourceList fromResourceList, ResourceList toResourceList)
{
    addStop(from, fromResourceList);
    addStop(to, toResourceList);
}

/////////////////
// Functions
/////////////////
// const_iterator getters for the internal container of stops
//   iterator dereferences to std::pair<FactoryKey, ResourceList>
//   so cbegin()->first gets key of first factory
//   and cend()-second gets RL of first factory
PairList<FactoryKey, ResourceList>::const_iterator Route::cbegin() const {
    return stops_.cbegin();
}
PairList<FactoryKey, ResourceList>::const_iterator Route::cend() const {
    return stops_.cend();
}

bool Route::addStop(FactoryKey factoryIndex, ResourceList allocated) 
{
    stops_.push_back(std::pair<FactoryKey, ResourceList>(factoryIndex, allocated));
    return true;
}

bool Route::addStop(FactoryKey factoryIndex, size_t index, ResourceList allocated) 
{
    if (index >= stops_.size()) // if the index is out of range, we will just add the stop to the end
    {
        //throw(std::out_of_range("Tried to add stop outside of route range"));
        return addStop(factoryIndex, allocated);
    }
    PairList<FactoryKey, ResourceList>::iterator position = stops_.begin();
    for (int i = 0; i < index; i++)
    {
        position++;
    }
    stops_.insert(position, std::pair<FactoryKey, ResourceList>(factoryIndex, allocated));
    return true;
}


// attempts to remove the given factor from the Route
// if removal succeeds, returns true
// else, returns false
bool Route::dropStop(FactoryKey factoryIndex, int past) 
{
    if(!(size() > 2)) {return false;} // enforce that a route cannot have less than two stops.
    if(past >= size()) {throw std::out_of_range("Attempted to drop factoryIndex outside of Route range");} // check for valid input

    PairList<FactoryKey, ResourceList>::iterator position = stops_.begin();
    for (int i = 0; i < past; i++)
    {
        position++;
    }
    while (position != stops_.end())
    {
        if (position->first == factoryIndex)
        {
            stops_.erase(position);
            return true;
        }
        position++;
    }
    return false;
}

// add up resources dropped off and picked up all stops to get net resourcelist
// to fit constraints, the resulting RL should have no negative quants
ResourceList Route::getNetResources() const {
    ResourceList rl;
    for(int i = 0; i < stops_.size(); i++) {
        rl += getResources(i);
    }
    return rl;
}

const ResourceList Route::getResources(size_t stop) const
{
    if(stop >= size()) {throw std::out_of_range("Attempted to getResources outside of Route range");} // check for valid input
    return stops_[stop].second;
}

const Quant Route::getResourceAmount(size_t stop, Resource resource) const
{
    if(stop >= size()) {throw std::out_of_range("Attempted to getResourcesAmount outside of Route range");} // check for valid input
    return stops_[stop].second[resource];
}

FactoryKey& Route::operator[](size_t stop)
{
    if (stop >= size()) 
    {
        throw std::out_of_range("Route stop out of range");
    }
    return stops_[stop].first;
}

size_t Route::size() const { 
    return stops_.size();
}

const bool Route::operator==(const Route& other) const
{
    if (size() != other.size()) {return false;}
    for (int i = 0; i < size(); i++)
    {
        if (stops_[i] != other.stops_[i] or getResources(i) != other.getResources(i)) {return false;}
    }
    return true;
}

const int Route::findStop(FactoryKey index) const
{
    int stop = -1;
    for (int i = 0; i < size(); i++)
    {
        if (stops_[i].first == index)
        {
            stop = i;
            return stop;
        }
    }
    return stop;
}

void Route::setResourceList(size_t stop, ResourceList updatedList)
{
    if(stop >= size()) {throw std::out_of_range("Attempted to setResourceList outside of Route range");} // check for valid input
    stops_[stop].second = updatedList;
}

void Route::setResource(size_t stop, Resource resource, Quant quantity)
{
    if(stop >= size()) {throw std::out_of_range("Attempted to setResource outside of Route range");} // check for valid input
    stops_[stop].second[resource] = quantity;
}

// Calculates which resources are effectively "carried over" from one cycle of the route to the next
ResourceList Route::getCarryover() const {
    // track resources currently carried by train
    ResourceList curr_resources;
    // add on positive Qs from first RL
    for(Resource r = static_cast<Resource>(0); r != Resource::COUNT; r++)
        if(stops_.front().second[r] > 0)
            curr_resources[r] += stops_.front().second[r];
    // for each stop after the first
    for(PairList<FactoryKey, ResourceList>::const_iterator it = stops_.cbegin(); ++it != stops_.cend(); ) {
        // add on RL
        curr_resources = curr_resources + it->second;
        // if RL would drop Q to negs, set Q to 0
        // the residue of these negative quants will be our carryover
        for(Resource r = static_cast<Resource>(0); r != Resource::COUNT; r++)
            if(curr_resources[r] < 0)
                curr_resources[r] = 0;
    }
    // to finish calculating carryover, apply negative quants from first factory
    for(Resource r = static_cast<Resource>(0); r != Resource::COUNT; r++)
        if(stops_.front().second[r] < 0)
            curr_resources[r] += stops_.front().second[r];
    
    // return final carryover, which would be used to cover the negative Qs we suppressed
    return curr_resources;
}

// get total length of track this route uses
Dist Route::getLength() const {
    // start with no distance
    Dist d = {0, 0};
    // track the location of previous stop
    // stop "before" first stop is the last stop
    d += dist(stops_.at(0).first, stops_.at(size()-1).first);
    // for each stop
    for(int i = 1; i < size(); i++) {
        // add dist between this stop and previous stop
        d += dist(stops_.at(i).first, stops_.at(i-1).first);
    }
    // return total distance
    return d;
}

// gets the number of factories that this route visits
size_t Route::getNumStops() const { 
    return size();
}

// CarryTime = Dist*Quant
// Measures the total distance resources are carried in the route

// order of ops for [F0, F1, F2, ..., Fn]
//  curr_resounces = carryover + (pos Qs from F0.RL)
//  for pairs (start, end) in [(F0, F1), (F1, F2), ..., (Fn, F0)]
//   curr_cap = sum over RL
//   d = dist(start, end)
//   carry_time += curr_cap * d;
//   curr_resounces += end.RL
double Route::getCarryTime() const {
    // running total of resources currently on the train
    ResourceList curr_resources = getCarryover();
    // running total of carry time
    double carry_time = 0;

    // add on positive Qs from first RL
    for(Resource r = static_cast<Resource>(0); r != Resource::COUNT; r++)
        if(stops_.front().second[r] > 0)
            curr_resources[r] += stops_.front().second[r];
    
    // for each stop after the first
    FactoryKey start_fact = stops_.front().first;
    bool loop_var = true;
    for(PairList<FactoryKey, ResourceList>::const_iterator end_it = ++stops_.cbegin(); loop_var; end_it++) {        
        // perform bound check
        if(end_it == stops_.cend()) {
            // mark loop ending
            loop_var = false;
            // set iter to start of array
            end_it = stops_.cbegin();
        }
        // calculate distance of this edge
        Dist d = dist(start_fact, end_it->first);

        // find curr cap
        Quant curr_cap = 0;
        // Qs in cap should never go negative
        for(Resource r = static_cast<Resource>(0); r != Resource::COUNT; r++)
            curr_cap += curr_resources[r];
        // update carry time by cap * dist
        carry_time += curr_cap * d.toDouble();

        // add on RL from end_it factory
        curr_resources = curr_resources + end_it->second;

        // update start funct
        start_fact = end_it->first;
    }
    
    // return carry time
    return carry_time;
}

// Returns the maximum number of resources carried by th route at any given time
//  this effectively represents the necesasry capacity of the train
Quant Route::getPeakCapacity() const {
    // running track of current max capacity
    Quant peak_cap = 0;
    // running total of resources currently on the train
    ResourceList curr_resources = getCarryover();

    // for each stop after the first
    for(PairList<FactoryKey, ResourceList>::const_iterator it = stops_.cbegin(); it != stops_.cend(); it++) {
        // add on capacity of this Factory
        // on first fact, only add positive
        if(it == stops_.cbegin()) {
            for(Resource r = static_cast<Resource>(0); r != Resource::COUNT; r++)
                if(it->second[r] > 0)
                    curr_resources[r] += it->second[r];
        // else add all
        } else {
            curr_resources = curr_resources + it->second;
        }

        // find current capacity
        Quant curr_cap = 0;
        // on second iter, all Qs in cap should always be 
        for(Resource r = static_cast<Resource>(0); r != Resource::COUNT; r++)
            curr_cap += curr_resources[r];

        // update max capacity
        peak_cap = std::max(peak_cap, curr_cap);
    }
    
    // return peak capacity
    return peak_cap;
}

void Route::reverse() {
    // no checks, cuz allocations aren't changing
    // reverse everything other than first stop
    std::reverse(++stops_.begin(), stops_.end());    
}

void Route::rotate(size_t n) {
    n %= stops_.size();

    std::rotate(stops_.begin(), stops_.begin()+n, stops_.end());
}