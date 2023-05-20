/*
Cost function declaration
Author: Ethan Worth, Andrew Bergman, Mason Paladino, Nozomu Ohno 11/19/22
Updated: Ethan Worth, Nozomu Ohno 11/29/22
First Draft Completed: Ethan Worth, 11/30/22

A cost function assignes a value to each network according to
chosen metrics like length of track, number of junctions, or 
anything else. Then we can compare two networks according to 
the cost function.

This file defines several cost functions and metrics.
*/

#ifndef COST_FUNCT_H
#define COST_FUNCT_H

/////////////////
// Includes
/////////////////

#include <stdint.h>
#include <array>
#include <map>
#include "../data/Network.h"

/////////////////
// Definitions
/////////////////

typedef double Cost;

/////////////////
// Cost function base
/////////////////

struct CostFunct
{
    // enum of every metric
        // Metric assigns cost to [...]
        // Units = [...]
        // Cost = W * (value of metric)   (W = weight associated with this metric)
    enum Metric { 
        // Metric assigns cost to number of junctions in the network
        // Units = size_t
        NumJunctions, 

        // Metric assigns cost per length of track in the network
        // Units = Dist.toDouble()
        BaseTrackLength, 

        // Metric assigns cost to additional routes sharing track length
        // Units = Dist.toDouble() * size_t
        // Cost calculated as sum over all shared sections
        //   for each shared section: C = W * (Shared Len) * (#Additional Routes)
        SharedTrackLength, 

        // Metric assigns cost to furthest track distance travelled by a route
        // Units = Dist.toDouble()
        MaxLength, 

        // Metric assigns cost to number of routes
        // Units = size_t
        NumRoutes, 

        // Metric assigns cost to maximum carry time of a route
        // Units = Dist.toDouble() * Quant
        MaxCarryTime, 

        // Metric assigns cost to total carry time of all routes
        // Units = Dist.toDouble() * Quant
        TotalCarryTime, 

        // Metric assigns cost to maximum peak capacity of any route
        // Units = Quant
        MaxPeakCapacity, 

        // Metric assigns cost to total peak capacity of all routes
        // Units = Quant
        TotalPeakCapacity,

        // dummy enum to track number of metrics
        COUNT 
    };
    // array of weights for each metric
    std::array<double, Metric::COUNT> weights;

    // base constructor
    CostFunct();
    // argumented constructor
    CostFunct(std::array<double, Metric::COUNT> ws);
    CostFunct(std::map<Metric, double> ws);
    
    // individual metric functions
    size_t getNumJunctions(const Network& net) const;
    std::map<size_t, Dist> getTrackLengthMap(const Network& net) const;
    Dist getMaxLength(const Network& net) const;
    size_t getNumRoutes(const Network& net) const;
    double getMaxCarryTime(const Network& net) const;
    double getTotalCarryTime(const Network& net) const;
    Quant getMaxPeakCapacity(const Network& net) const;
    Quant getTotalPeakCapacity(const Network& net) const;

    // calculate cost of network
    Cost operator()(const Network& net) const;
    // Cost getMetric(Metric m, const Network& net) const;
};

// increment operators for Metric enum
CostFunct::Metric& operator++(CostFunct::Metric& m);
CostFunct::Metric operator++(CostFunct::Metric& m, int);

#endif