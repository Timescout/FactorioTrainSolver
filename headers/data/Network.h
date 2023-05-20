/*
Network Class declaration
Author: Ethan Worth, Andrew Bergman, Mason Paladino, Nozomu Ohno
11/19/22

A Network holds together all of the information about factories,
routes, and junctions as well as how they relate to one another.
It is the highest level of data structure used in this project.
*/

#ifndef NETWORK_H
#define NETWORK_H

/////////////////
// Includes
/////////////////

#include "../data/Route.h"
#include "../data/Factory.h"
#include <vector>
#include <unordered_map>

/////////////////
// Definitions
/////////////////

typedef uint32_t RouteKey;

/////////////////
// Network Class
/////////////////

class Network {
private:
    // places_ stores factories and junctions.
    // Factories are distinguished by producing or consuming something,
    // whereas junctions do not produce or consume anything.
    std::map<FactoryKey, Factory, LocationCompare> places_;
    std::vector<Route> routes_;      // stores route objects

public:

    // constructor
    Network();
    Network(std::vector<Factory> facts);

    // getters for size
    const size_t getNumFactories() const;  // returns number of factories
    const size_t getNumJunctions() const;  // returns number of junctions
    const size_t getNumRoutes() const;     // returns number of routes

    // getters for container iterators
    std::map<FactoryKey, Factory, LocationCompare>::const_iterator factCBegin() const; // use to iterate over Location, Factory pairs
    std::map<FactoryKey, Factory, LocationCompare>::const_iterator factCEnd() const;
    std::vector<Route>::const_iterator routeCBegin() const; // use to iterate over routes.
    std::vector<Route>::const_iterator routeCEnd() const;

    // getters for elements
    const Factory& getPlace(FactoryKey key) const; // returns factory at given index
    const Route& getRoute(RouteKey index) const; // returns Route at given index
    bool hasPlace(FactoryKey key) const;


    // erase the place at the given key, no matter what it is.
    // will also remove key from all routes that it is a part of,
    // and will return false if that removal fails.
    // recommended that we don't use this because it is very slow
    // unless we absolutly have to.
    bool erasePlace(FactoryKey key);
    bool erasePlace(Factory factory);
    bool erasePlace(Coord x, Coord y);
    
    // getters that interface with routes
    std::vector<FactoryKey> getRouteStops(RouteKey index) const;
    FactoryKey getStop(RouteKey route, size_t stop) const;
    ResourceList getStopCommand(RouteKey route, size_t stop) const;
    
    bool addStop(RouteKey route, FactoryKey factory, size_t routePosition, ResourceList command = ResourceList());
    bool setStopCommand(RouteKey route, size_t stop, ResourceList newCommand);
    bool dropStop(RouteKey routeKey, FactoryKey Factorykey, int past = 0);

    void addFactory(Factory factory);
    void addFactory(Location loc, ResourceList production);
    void addFactory(Coord x, Coord y, ResourceList production);

    // Erase A factory. Reccommended to use when a factory is 
    // what you want to erase. Reccomended that you don't get
    // into a senario where you want to erase a factory.
    bool eraseFactory(Factory factory);
    bool eraseFactory(FactoryKey key);

    // check to make sure a juction at that point does not already exist
    bool createJunct(Location loc);
    bool createJunct(Coord x, Coord y);

    // Erase A junction. Reccommended to use when a junction is 
    // what you want to erase. Reccomended that you don't get
    // into a senario where you want to erase a junction.
    bool eraseJunction(Factory juction);
    bool eraseJunction(FactoryKey key);

    // adds a route to the network with the given factories and commands
    // must contain at least two pairs to succede, returns false if not met
    // the factories must be able to accept the commands, if they cannot
    // returns false
    // returns true if the route was created successfully.
    // takes a PairList
    bool addRoute(PairList<FactoryKey, ResourceList> initalPairs);
    // takes a vector of factories and a vector of commandscommands.
    bool addRoute(std::vector<FactoryKey> initialFactories, std::vector<ResourceList> initialCommands = std::vector<ResourceList>(2, ResourceList()));
    // takes two factories and two commands, which default to empty commands.
    bool addRoute(FactoryKey from, FactoryKey to, ResourceList commandFrom = ResourceList(), ResourceList commandTo = ResourceList());
    // Adds a route
    bool addRoute(Route route);

    bool eraseRoute(Route route);
    bool eraseRoute(RouteKey key);
    void eraseAllRoutes();


    // return a list of factory indecies with a deficit of the given resource
    // if no resource is given then will give all factories with any deficit
    std::vector<FactoryKey> getDeficit(Resource resource = Resource::COUNT) const;
    // return a list of factory indecies with a surplus of the given resource
    // if no resource is given then will give all factories with any surplus
    std::vector<FactoryKey> getSurplus(Resource resource = Resource::COUNT) const;

    //////////////////
    // atomic mutaters
    //////////////////
    
    // reverse order of stops, without changing start
    void reverseRoute(RouteKey key);
    // shift start to the nth element in the stops_ list
    //   default: shift by 1
    void rotateRoute(RouteKey key, size_t n = 1);

    //bool combineRoutes(RouteKey k1, RouteKey k2, size_t i1, size_t i2);
};

#endif