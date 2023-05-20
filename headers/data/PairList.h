/*
Factory Class definition
Author: Ethan Worth, Andrew Bergman, Mason Paladino, Nozomu Ohno
11/19/22
*/

#ifndef PAIR_LIST_H
#define PAIR_LIST_H

/////////////////
// Includes
/////////////////

#include <deque>
#include <utility>

/////////////////
// Pairilist define
/////////////////

// currently PairList is just an alias for a map
// however, it gets its own file in case we want to give it its own design later
template <typename P1, typename P2>
using PairList = std::deque<std::pair<P1, P2>>;

#endif