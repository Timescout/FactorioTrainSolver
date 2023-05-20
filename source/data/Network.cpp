/*
Network Class definitions
Author: Ethan Worth, Andrew Bergman, Mason Paladino, Nozomu Ohno
11/19/22

Definitions for the Network class. A network pulls together all of the other data 
structures we use in this project and manages them toghether. It is the high level
interface for these and it is reccommended to use Network rather than the individual
classes themselves. Network takes care of many constraints and management on its own.
*/

/////////////////
// Includes
/////////////////
#include "../../headers/data/Network.h"


/////////////////
// Constructors
/////////////////

Network::Network()
{}

Network::Network(std::vector<Factory> facts) : 
    routes_()
    {
        for(int i = 0; i < facts.size(); i++)
        {
            places_.insert(std::pair<FactoryKey, Factory>(facts[i].getLoc(), facts[i]));
        }
    }

/////////////////
// Functions
/////////////////

const size_t Network::getNumFactories() const
{
    size_t size = 0;
    for (std::map<FactoryKey,Factory,LocationCompare>::const_iterator i = places_.cbegin(); i != places_.cend(); i++)
    {
        if(!(i->second.getBaseQuants() == ResourceList()))
        {
            size++;
        }
    }
    return size;
}

const size_t Network::getNumJunctions() const
{
    size_t size = 0;
    for (std::map<FactoryKey,Factory,LocationCompare>::const_iterator i = places_.cbegin(); i != places_.cend(); i++)
    {
        if(i->second.getBaseQuants() == ResourceList())
        {
            size++;
        }
    }
    return size;
}

const size_t Network::getNumRoutes() const
{
    return routes_.size();
}

// getters for container iterators
std::map<FactoryKey, Factory, LocationCompare>::const_iterator Network::factCBegin() const {
    return places_.cbegin();
}
std::map<FactoryKey, Factory, LocationCompare>::const_iterator Network::factCEnd() const {
    return places_.cend();
}
std::vector<Route>::const_iterator Network::routeCBegin() const {
    return routes_.cbegin();
}
std::vector<Route>::const_iterator Network::routeCEnd() const {
    return routes_.cend();
}

const Factory& Network::getPlace(FactoryKey key) const
{
    //if (index >= places_.size()) {throw std::out_of_range("FactoryKey out of range");}
    return places_.at(key);
}

const Route& Network::getRoute(RouteKey key) const
{
    return routes_.at(key);
}

bool Network::hasPlace(FactoryKey key) const
{
    return places_.find(key) != places_.cend();
}

bool Network::erasePlace(FactoryKey key)
{
    // erasePlace must erase key from all routes, and the places_ map
    if (places_.find(key) == places_.end()) { return false; } // check for valid key

    // if the erasePlace fails, we have to undo all of the erasing that
    // we have done up till now. The easiest way is to backup the data
    // before we start eraseing it and restore it if we have to.
    std::map<FactoryKey, Factory, LocationCompare> backupPlaces = places_;
    std::vector<Route> backupRoutes = routes_;
    for(int i = 0; i < getNumRoutes(); i++)
    {
        while (getRoute(i).findStop(key) >= 0) // a place may be in a Route multiple times
        {
            if (!dropStop(i, key)) // if the drop fails, then the whole erase fails.
            {
                places_ = backupPlaces;
                routes_ = backupRoutes;
                return false;
            } 
        }
    }
    return places_.erase(key);
}

bool Network::erasePlace(Factory factory)
{
    return erasePlace(factory.getLoc());
}

bool Network::erasePlace(Coord x, Coord y)
{
    return erasePlace(Location(x,y));
}

std::vector<FactoryKey> Network::getRouteStops(RouteKey index) const
{
    std::vector<FactoryKey> stops;
    for (auto i = routes_[index].cbegin(); i != routes_[index].cend(); i++)
    {
        stops.push_back(i->first);
    }
    return stops;
}

FactoryKey Network::getStop(RouteKey route, size_t stop) const
{
    // check valid input
    if (stop >= routes_[route].size()) {throw std::out_of_range("stop out of route range");}
    return (routes_[route].cbegin()+stop)->first;
}

ResourceList Network::getStopCommand(RouteKey route, size_t stop) const{
    return routes_.at(route).getResources(stop);
}

bool Network::addStop(RouteKey route, FactoryKey factory, size_t routePosition, ResourceList command)
{
    if (places_.find(factory) == places_.end()) {return false;} // check for valid input
    if (!places_.at(factory).allocate(command)){return false;} // try to allocate the command at the factory
    return routes_[route].addStop(factory, routePosition, command);
}

bool Network::setStopCommand(RouteKey route, size_t stop, ResourceList newCommand)
{
    // check for valid input
    if (routes_.size() <= route) {return false;} 
    if (routes_.at(route).size() <= stop) {return false;} 
    // store old command in case we need to reset it later
    ResourceList oldCommand = routes_[route].getResources(stop);
    places_.at(routes_[route][stop]).deallocate(oldCommand); // deallocate the old command
    if (!places_.at(routes_[route][stop]).allocate(newCommand)) // try to allocate the new command
    {
        // if the newCommand fails to allocate, restore the system to the old command.
        places_.at(routes_[route][stop]).allocate(oldCommand);
        return false;
    }
    // finally, set the newCommand in the Route
    routes_[route].setResourceList(stop, newCommand);
    return true;
}

bool Network::dropStop(RouteKey routeKey, FactoryKey factoryKey, int past)
{
    return routes_[routeKey].dropStop(factoryKey, past);
}

void Network::addFactory(Factory factory)
{
    places_.insert(std::pair<FactoryKey,Factory>(factory.getLoc(), factory));
}

void Network::addFactory(Location loc, ResourceList production)
{
    Factory factory(loc, production);
    addFactory(factory);
}

void Network::addFactory(Coord x, Coord y, ResourceList production)
{
    Factory factory(x, y, production);
    addFactory(factory);
}

bool Network::eraseFactory(Factory factory)
{
    if(factory.getBaseQuants().empty()) return false; // enforce that this function can Only erase factories, not junctions
    return Network::erasePlace(factory);
}

bool Network::eraseFactory(FactoryKey key)
{
    if (places_.find(key) == places_.end()) {return false;} // if the key is not found, the erase fails and returns
    return eraseFactory(getPlace(key));
}

bool Network::createJunct(Location location)
{
    addFactory(location, ResourceList());
    return true;
}

bool Network::createJunct(Coord x, Coord y)
{
    addFactory(x, y, ResourceList());
    return true;
}

bool Network::eraseJunction(Factory junciton)
{
    if (!(junciton.getBaseQuants() == ResourceList())) // enforce that this function can only erase juctions, not factories
    {
        return false;
    }
    return erasePlace(junciton);
}

bool Network::eraseJunction(FactoryKey key)
{
    if (places_.find(key) == places_.end()) {return false;} // if the key is not found, the erase fails and returns
    return eraseJunction(getPlace(key));
}

bool Network::addRoute(std::vector<FactoryKey> initialFactories, std::vector<ResourceList> initialCommands)
{
    PairList<FactoryKey,ResourceList> pairs;
    if (initialCommands.size() < initialFactories.size()) // if there are not enough initial commands, add blank ResourceLists
    {
        for (int i = 0; i < initialFactories.size() - initialCommands.size(); i++) 
        {
            initialCommands.push_back(ResourceList());
        }
    }
    for (int i = 0; i < initialFactories.size(); i++)
    {
        pairs.push_back(std::pair<FactoryKey, ResourceList>(initialFactories[i], initialCommands[i]));
    }
    return addRoute(pairs);
}

bool Network::addRoute(FactoryKey from, FactoryKey to, ResourceList commandFrom, ResourceList commandTo)
{
    PairList<FactoryKey,ResourceList> pairs
    {
        {from, commandFrom},
        {to, commandTo}
    };
    return addRoute(pairs);
}

bool Network::addRoute(PairList<FactoryKey, ResourceList> initialPairs)
{
    if (initialPairs.size() < 2) {return false;} // enforce Route minimum size
    Route r(initialPairs);
    return addRoute(r);
}

bool Network::addRoute(Route route){
    // by the time we have created a route, it will have at least two stops.
    for (int i = 0; i < route.size(); i++)
    {
        if (!places_[route[i]].allocate(route.getResources(i))) // check that the Command can be executed
        { // if it can't, we have to reset the network to the state it was in before we tried to add this
          // route, by deallocating all allocated resources up till now.
            for (int ii = 0; ii < i; ii++) // aye aye aye!
            {
                places_[route[ii]].deallocate(route.getResources(ii));
            }
            return false; // addRoute fails
        }
    }
    routes_.push_back(route);
    return true; // addRoute Success
}

bool Network::eraseRoute(Route route)
{
    //Finds route and deletes it from vector
    //Returning true inside the for loop assumes that the route is unique otherwise it will only delete the first one
    for(int i = 0; i < routes_.size(); i++){
        if(routes_[i] == route) { return eraseRoute(i); }
    }
    //Return false if didn't delete anything
    return false;
}

bool Network::eraseRoute(RouteKey key)
{
    // check for valid input
    if (key >= routes_.size()) {return false;}
    // iterate over the route, and deallocate the resources at each stop
    for(int i = 0; i < routes_[key].size(); i++)
    {
        places_[routes_[key][i]].deallocate(routes_[key].getResources(i)); // should not fail
        routes_[key].setResourceList(i, ResourceList()); // set the command to empty
    }
    // finally, erase the route
    routes_.erase(routes_.begin()+key);
    return true;
}

void Network::eraseAllRoutes() {
    while(getNumRoutes())
        eraseRoute(0);
}

void Network::reverseRoute(RouteKey key) {
    // no checks, cuz allocations aren't changing
    routes_.at(key).reverse();
}
void Network::rotateRoute(RouteKey key, size_t n) {
    // no checks, cuz allocations aren't changing
    routes_.at(key).rotate(n);
}
/*bool Network::combineRoutes(RouteKey k1, RouteKey k2, size_t i1, size_t i2) {
    // can only combine on matching stop
    if(!(getStop(k1, i1) == getStop(k2, i2)))
        return false;

    Route r1 = getRoute(k1), r2 = getRoute(k2); 
    // add on RLs at combo: k1[i1].second += k2[i2].second

    return true;
}*/

std::vector<FactoryKey> Network::getDeficit(Resource resource) const
{
    std::vector<FactoryKey> hasDeficit;
    if (resource != Resource::COUNT)
    {
        for (auto i = places_.cbegin(); i != places_.cend(); i++)
        {
            if (i->second.getUnallocated()[resource] < 0)
            {
                hasDeficit.push_back(i->first);
            }
        }
    }
    else
    {
        for (auto i = places_.cbegin(); i != places_.cend(); i++)
        {
            for(Resource r = Resource(0); r != Resource::COUNT; r++)
            {
                if (i->second.getUnallocated()[r] < 0)
                {
                    hasDeficit.push_back(i->first);
                    break;
                }
            }
        }
    }
    return hasDeficit;
}

std::vector<FactoryKey> Network::getSurplus(Resource resource) const
{
        std::vector<FactoryKey> hasSurplus;
    if (resource != Resource::COUNT)
    {
        for (auto i = places_.cbegin(); i != places_.cend(); i++)
        {
            if (i->second.getUnallocated()[resource] > 0)
            {
                hasSurplus.push_back(i->first);
            }
        }
    }
    else
    {
        for (auto i = places_.cbegin(); i != places_.cend(); i++)
        {
            for(Resource r = Resource(0); r != Resource::COUNT; r++)
            {
                if (i->second.getUnallocated()[r] > 0)
                {
                    hasSurplus.push_back(i->first);
                    break;
                }
            }
        }
    }
    return hasSurplus;
}
