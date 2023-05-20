# UML
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
Calculates cost of given network (stores set of weights?)
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
