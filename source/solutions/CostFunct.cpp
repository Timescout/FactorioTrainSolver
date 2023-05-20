/*
Cost function definitions
Author: Ethan Worth, Andrew Bergman, Mason Paladino, Nozomu Ohno 11/19/22
Updated: Ethan Worth, Nozomu Ohno 11/29/22
First Draft Completed: Ethan Worth, 11/30/22

Definitions for CostFunct's
*/

/////////////////
// Includes
/////////////////
#include "../../headers/solutions/CostFunct.h"
#include <math.h>

/////////////////
// Metric Functions
/////////////////
CostFunct::Metric& operator++(CostFunct::Metric& m) {
    return m = static_cast<CostFunct::Metric>((m+1) % (CostFunct::Metric::COUNT+1));
}
CostFunct::Metric operator++(CostFunct::Metric& m, int) {
    CostFunct::Metric orig = m;
    m = static_cast<CostFunct::Metric>((m+1) % (CostFunct::Metric::COUNT+1));
    return orig;
}

///////////////
// Constructor
///////////////
CostFunct::CostFunct(){}

CostFunct::CostFunct(std::array<double, CostFunct::Metric::COUNT> ws) 
    : weights(ws) {}

CostFunct::CostFunct(std::map<Metric, double> ws) 
    : weights() {
    for(std::pair<Metric, double> w : ws)
        weights[w.first] = w.second;
}

/////////////////
// Metric Getters
/////////////////

size_t CostFunct::getNumJunctions(const Network& net) const {
    return net.getNumJunctions();
}

// O(r * f_r)
std::map<size_t, Dist> CostFunct::getTrackLengthMap(const Network& net) const {
    // map a unique pair of Locations to the number of Route using that connection
    std::map<std::pair<FactoryKey, FactoryKey>, size_t> edge_mult_map;
    
    // fill edge map
    // for each Route
    for(size_t i = 0; i < net.getNumRoutes(); i++) {
        Route r = net.getRoute(i);
        // Factory "previous" to first fact is the last fact
        FactoryKey prev_fk = (--r.cend())->first;
        // for each fact
        for(PairList<FactoryKey, ResourceList>::const_iterator fp_it = r.cbegin(); fp_it != r.cend(); fp_it++) {
            // add edge
            edge_mult_map[std::make_pair(prev_fk, fp_it->first)]++;
            // update prev
            prev_fk = fp_it->first;
        }
    }

    // convert edge map to length map
    std::map<size_t, Dist> out;
    for(std::pair<std::pair<FactoryKey, FactoryKey>, size_t> mp : edge_mult_map)
        out[mp.second] += dist(mp.first.first, mp.first.second);
    // return length map
    return out;
}

// O(r * f_r)
Dist CostFunct::getMaxLength(const Network& net) const {
    // track best length
    Dist m = {0, 0};
     // for each Route
    for(size_t i = 0; i < net.getNumRoutes(); i++) {
        // get len
        Dist len = net.getRoute(i).getLength();
        // track best len
        if(m.toDouble() < len.toDouble())
            m = len;
    }
    // return best route
    return m;
}

size_t CostFunct::getNumRoutes(const Network& net) const {
    return net.getNumRoutes();
}

// O(r * f_r)
double CostFunct::getMaxCarryTime(const Network& net) const {
    // running track of current max carry time
    double carry_time = 0;

    // for each Route
    for(size_t i = 0; i < net.getNumRoutes(); i++)
        carry_time = std::max(carry_time, net.getRoute(i).getCarryTime());
    
    // return max peak capacity
    return carry_time;    
}

// O(r * f_r)
double CostFunct::getTotalCarryTime(const Network& net) const {
    // running track of current total carry time
    double carry_time = 0;

    // for each route
    for(size_t i = 0; i < net.getNumRoutes(); i++)
        carry_time += net.getRoute(i).getCarryTime();
    
    // return total carry time
    return carry_time;
}

// O(r * f_r)
Quant CostFunct::getMaxPeakCapacity(const Network& net) const {
    // running track of current max capacity
    Quant peak_cap = 0;

    // for each route
    for(size_t i = 0; i < net.getNumRoutes(); i++)
        peak_cap = std::max(peak_cap, net.getRoute(i).getPeakCapacity());
    
    // return max peak capacity
    return peak_cap;
}

// O(r * f_r)
Quant CostFunct::getTotalPeakCapacity(const Network& net) const {
    // running track of current total capacity
    Quant total_cap = 0;

    // for each route
    for(size_t i = 0; i < net.getNumRoutes(); i++)
        total_cap += net.getRoute(i).getPeakCapacity();
    
    // return total peak capacity
    return total_cap;
}

///////////////
// Functions
///////////////
/*Cost CostFunct::getMetric(Metric m, const Network& net) const {
    switch (m) {
    case NumJunctions:
        return getNumJunctions(net) * weights[m];
    }
    return 0;
}*/

// O(r * f_r)
Cost CostFunct::operator()(const Network& net) const {
    Cost c = 0;
    // NumJunctions 
    c += getNumJunctions(net) * weights[NumJunctions];

    // BaseTrackLength, SharedTrackLength
    std::map<size_t, Dist> sharing_record = getTrackLengthMap(net);
    for (std::pair<size_t, Dist> sr : sharing_record)
        // Cost(Dist, NumRoutes) = D * (BaseCost * (NR - 1)*(1 - Discount))
        //   first route is charged at base rate
        //   all additional routes apply a discount
        c += sr.second.toDouble() * (weights[BaseTrackLength] + (sr.first - 1)*(weights[SharedTrackLength]));
    
    // MaxLength
    c += getMaxLength(net).toDouble() * weights[MaxLength];

    // NumRoutes
    c += getNumRoutes(net) * weights[NumRoutes];

    // NumRoutes
    c += getMaxCarryTime(net) * weights[MaxCarryTime];

    // NumRoutes
    c += getTotalCarryTime(net) * weights[TotalCarryTime];

    // NumRoutes
    c += getMaxPeakCapacity(net) * weights[MaxPeakCapacity];

    // NumRoutes
    c += getTotalPeakCapacity(net) * weights[TotalPeakCapacity];
    return c;
}
