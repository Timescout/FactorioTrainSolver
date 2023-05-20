# Factories and Train Networks
Members: Andrew Bergman, Mason Paladino, Nozomu Ohno, Ethan Worth

## Description of the Project

### Purpose of the Project
Figure out the most efficient route for trains to run between the factories while meeting certain constrains. Each factory has resources that it produces and resources that it consumes. The entire network consists of one or more routes that connects all factories and moves resources between them.

### Goal of the Project
Create a program and come up with the algorithm to calculate the most efficient route between the factories that satisfies certain constraints that we have cretated.

In order to calculate the effieciency of the route, we came up with costs that occur in a network.
Some of these costs include: 
* \# of junctions
* various track lengths
* \# of routes
* Various resource carrying metrics

Some constraints that we have added in order for a route to be valid are:
* Route does not have the same factory twice in a row.
* All factories in the route must have their demand of resources met.
* Sum of all resources required or produced by the factories in the route must be greater than or equal to 0.

### Data Model Overview
Factories are an object with a Location in the coordinate plane, and a set of Resources. Positive values in this set represent Resources that are produced at that factory, while negative values represent Resources that are consumed. A Route is a sequence of routes that represents a train with those stops. A route also has a list of commands which keeps track of what resources are picked up and dropped of at each stop along the route Track is built along routes. Junctions are places that allow routes to intersect and share track. 

In the implemented data model, Network maintains a map of Locations to places, which may be Factories or Junctions. Junctions are just factories that don't consume or produce any resources. A ResourceList is a vector of Resources that can represent supply/demand, a command to drop off or pick up at a stop, or how much a factory has remaning. Routes are a list of Location/ResourceList pairs where the resourcelist holds a command for the train to execute at the location. This location may refer to a Junction or Factory. Factories keep track of how much of their production/consumption is satisfied but not which Routes satisfy it, Network keeps track of all that. You should have to only deal with Network in our data model.

### Algorithmic Apporoach Used to Solve the Problem
We have decided to use the genetic algorithm to solve the problem stated above. We start with a network that has a solution, then we have the program mutate the routes until the most efficient route has been created. 
 - ## TODO: Ethan can take this

## How to Run the Program
 - ## TODO: NOtes about `tasks.json`

## Assumptions Used in the Project/Decisions made about forming the problem
* The factories are all placed on a discrete Euclidian grid
* Obstacles do not exist on the grid
* Factories cannot produce and require same type of resource
* Junction does not produce any resources nor require any resources
* There can only be one factory or a junction on each coordinate point
* Tracks can only go in cardinal and ordinal directions (8 directions)

## Pseudocode

## Works Cited
* CPlusPlus.com
* ISO CPP Guidelines: https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
* Increment/Decrement Operators for `enum`: https://stackoverflow.com/a/2571531
* Custom Sign Tracker: https://stackoverflow.com/a/4609795



# Variables Used in Complexity Analysis
 - f - number of factories
 - r - number of routes
 - f_r - number of factories in a route
 - R - number of resources (constant)
## Rules/Assumptions
 - `r * f_r ~= c*f`
   - Factories are probably used by around a constant number of Routes
 - r is linearly related to f (esp for GEL)