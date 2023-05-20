#ifndef TEST_SETUP_H
#define TEST_SETUP_H

#include <random>
#include <time.h>

const uint16_t MAX_RAND_TESTS = 100,
               MAX_NET_SIZE = 100,
               MAX_LOC_COORD = 1000,
               MIN_LOC_COORD = -1000;

void setupTests() {
    srand(time(0));
}

#endif