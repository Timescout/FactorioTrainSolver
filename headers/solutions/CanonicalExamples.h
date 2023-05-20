#ifndef CANONICAL_EXAMPLES_H
#define CANONICAL_EXAMPLES_H

#include "../data/Network.h"
#include <random>
#include "CostFunct.h"


const CostFunct ALL_COSTS({
    {CostFunct::Metric::NumJunctions,      4.0},
    {CostFunct::Metric::BaseTrackLength,   1.0},
    {CostFunct::Metric::SharedTrackLength, 0.5},
    {CostFunct::Metric::MaxLength,         0.5},
    {CostFunct::Metric::NumRoutes,         2.0},
    {CostFunct::Metric::MaxCarryTime,      0.5},
    {CostFunct::Metric::TotalCarryTime,    0.1},
    {CostFunct::Metric::MaxPeakCapacity,   0.5},
    {CostFunct::Metric::TotalPeakCapacity, 0.1}
}), SIMPLE_COSTS({
    {CostFunct::Metric::NumJunctions,      4.0},
    {CostFunct::Metric::BaseTrackLength,   1.0},
    {CostFunct::Metric::SharedTrackLength, 0.5},
    {CostFunct::Metric::NumRoutes,         2.0}
});

const Network 
// CANON_BASIC - simple two-factory produce consumer
CANON_BASIC((std::vector<Factory>) {
    Factory(
        (Location){0, 0}, 
        ResourceList((std::map<Resource, Quant>) {
            {Resource::Copper, 1}
        })
    ), Factory(
        (Location){0, 1}, 
        ResourceList(
            (std::map<Resource, Quant>) {
            {Resource::Copper, -1}
        })
    )
}),
// CANON_DUAL_SERVE - single producer serving two (lopsided) consumers
CANON_DUAL_SERVE((std::vector<Factory>) {
    Factory(
        (Location){0, 0}, 
        ResourceList((std::map<Resource, Quant>) {
            {Resource::Copper, 3}
        })
    ), Factory(
        (Location){2, 1}, 
        ResourceList((std::map<Resource, Quant>) {
            {Resource::Copper, -1}
        })
    ), Factory(
        (Location){1, 2}, 
        ResourceList((std::map<Resource, Quant>) {
            {Resource::Copper, -2}
        })
    )
}),
// CANON_DUAL_RES_PRODUCE - single factory produces two resources, each consumed by one consumer
CANON_DUAL_RES_PRODUCE((std::vector<Factory>) {
    Factory(
        (Location){0, 0}, 
        ResourceList((std::map<Resource, Quant>) {
            {Resource::Copper, -2},
            {Resource::Iron, -2 }
        })
    ), Factory(
        (Location){2, 1}, 
        ResourceList((std::map<Resource, Quant>) {
            {Resource::Copper, 2}
        })
    ), Factory(
        (Location){1, 2}, 
        ResourceList((std::map<Resource, Quant>) {
            {Resource::Iron, 2}
        })
    )
}), 
// CANON_TWO_ZONES - two zones, each of which contains factories which would 
CANON_TWO_ZONES((std::vector<Factory>) {
    Factory(
        (Location){0, 0}, 
        ResourceList((std::map<Resource, Quant>) {
            {Resource::Copper, -4},
            {Resource::Circuits, 2}
        })
    ), Factory(
        (Location){0, 2}, 
        ResourceList((std::map<Resource, Quant>) {
            {Resource::Uranium, -1},
            {Resource::NuclearFuel, 4}
        })
    ),
    Factory(
        (Location){100, 0}, 
        ResourceList((std::map<Resource, Quant>) {
            {Resource::Copper, 4},
            {Resource::Circuits, -2}
        })
    ), Factory(
        (Location){100, 2}, 
        ResourceList((std::map<Resource, Quant>) {
            {Resource::Uranium, 1},
            {Resource::NuclearFuel, -4}
        })
    )
}), CANON_TRI_CYCLE((std::vector<Factory>) {
    Factory(
        (Location){0, 0}, 
        ResourceList((std::map<Resource, Quant>) {
            {Resource::Copper, 0},
            {Resource::Iron, 1 },
            {Resource::Steel, -1 }
        })
    ), Factory(
        (Location){2, 1}, 
        ResourceList((std::map<Resource, Quant>) {
            {Resource::Copper, -1},
            {Resource::Iron, 0 },
            {Resource::Steel, 1 }
        })
    ), Factory(
        (Location){1, 2}, 
        ResourceList((std::map<Resource, Quant>) {
            {Resource::Copper, 1},
            {Resource::Iron, -1 },
            {Resource::Steel, 0 }
        })
    )
});

//Generate a random network to solve
//Auto populates the given network with random factories
//seed will be used in srand for the random number generator
//If network is populated, clear the object of all data first
Network randomNetwork(int seed = rand(), int NumFactories = 10, int maxCoord = 10);

#endif