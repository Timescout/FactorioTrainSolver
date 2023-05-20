# Steps
## Meeting on 11/12
we want to make a working model (not an alg) which can
 - store a set of factories
 - model routes
 - verify constraints
 - calculate cost

## Week of 11/14
 - UML (NO, EW)
 - `.h` files (NO, EW)
 - `.cpp` files (AB, MP)

## Meeting on 11/27
 - Created comparison operator for distance
 - Started on implementing solution classes 
## Week of 11/28
 - All of data implemented and tested (Unit Test: All)
   - Add Unit Test for: 
     - Factory
     - Resource/ResourceList
     - Route
 - Implement a brute-force/heuristic algorithm (All)
 - Implement solution classes (NO, EW)
 - Enumerate canonical networks
# Psuedo-UML
## Dependency Tiers
Lower items do not depend on higher items
 - Alg
   - Solver
 - Eval
   - CostFunct, Constraints
 - Data
   - Network
   - Route
   - Factory
   - Location
## `class Solver`
Generates routes over a network, evaluates solution
```
Network
CostFunct
Constraints
```

## `class CostFunct`
Calculates cost of given network (sotres set of weights?)
```
Weights?
Cost operator(Network n)
```

## `class Constraints`
Determines if network fulfills constraints
```
bool operator(Network n)
```

## `class Network`
Store information about a network
```cpp
vector<Factory> factories;
vector<Factory> juncts; // no input/output
map<RouteID, Route> routes;
```

## `struct Route`
```cpp
// typedef for RouteID
RouteID id;
list<Factory*> facts;
```

## `class Factory`
```cpp
// enum Resource
// typedefs for Quant, Coord
Location loc;
map<Resource,Quant> quants;
map<Resource,map<RouteID,Quant>> allocated;
```

## `struct Location`
```cpp
Coord x,y;
```

# Constraints and Cost
Constraints replace optimization - instead of putting something in the cost function, we make it a constraint
## Constraints
Solution is only valid if it fulfills all constraints
 - the same factory cannot be visited twice in a row, cannot go from a factory to itself.
 - routes must be simple cycles
 - at each factory on a route, the route may allocate some quanitity of resource to be produced/consumed at that factory
   - sum of allocated production cannot exceed maximum
     - a valid solution can leave production unallocated
   - sum of allocated consumption must EQUAL desired consumption
     - a valid solution must fully satisfy every consumer
 - for every route, every resource must fulfill: (production_alloc - consumption_alloc >= 0)
   - cannot consume more than you produce
   - can produce more than you consume (leftover resource is wasted, that's a cost)
 - what if the total amount of a resource produced is less than the amount consumed? then the netwrok can't satisfy consumers and no valid solution exists
   - Workaround: run certain factories at reduced speed to lower the consumption and production
     - Additional problem constraint: to make this evaluation simpler, the resource dependency graph should be acyclic

## Cost
Goal: create function that determines cost of route, cost of total network is cost of all routes
 - track length
 - wasted resource
 - route speed
