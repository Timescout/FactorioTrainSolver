#include <gtest/gtest.h>
#include "../headers/data/Location.h"
#include "TestSetup.h"
#include <random>

TEST(DistTest, NoDist) {
    EXPECT_EQ(dist((Location){0, 0}, (Location){0, 0}), ((Dist){0, 0}));
    EXPECT_EQ(dist((Location){-1, -1}, (Location){-1, -1}), ((Dist){0, 0}));
}

TEST(DistTest, RatOnly) {
    EXPECT_EQ(dist((Location){0, 0}, (Location){0, 5}), ((Dist){5, 0}));
    EXPECT_EQ(dist((Location){0, 0}, (Location){5, 0}), ((Dist){5, 0}));
    EXPECT_EQ(dist((Location){10, 0}, (Location){5, 0}), ((Dist){5, 0}));
    EXPECT_EQ(dist((Location){0, 10}, (Location){0, 5}), ((Dist){5, 0}));
}

TEST(DistTest, IrratOnly) {
    EXPECT_EQ(dist((Location){0, 0}, (Location){5, 5}), ((Dist){0, 5}));
    EXPECT_EQ(dist((Location){5, 5}, (Location){0, 0}), ((Dist){0, 5}));
    EXPECT_EQ(dist((Location){5, 0}, (Location){0, 5}), ((Dist){0, 5}));
    EXPECT_EQ(dist((Location){0, 5}, (Location){5, 0}), ((Dist){0, 5}));
}

TEST(DistTest, RatIrrat) {
    EXPECT_EQ(dist((Location){0, 0}, (Location){1, 2}), ((Dist){1, 1}));
    EXPECT_EQ(dist((Location){1, 2}, (Location){0, 0}), ((Dist){1, 1}));
    EXPECT_EQ(dist((Location){0, 0}, (Location){3, 7}), ((Dist){4, 3}));
    EXPECT_EQ(dist((Location){3, 7}, (Location){0, 0}), ((Dist){4, 3}));
    EXPECT_EQ(dist((Location){7, 2}, (Location){5, 5}), ((Dist){1, 2}));
    EXPECT_EQ(dist((Location){5, 5}, (Location){7, 2}), ((Dist){1, 2}));
}

// 50 random location pairs and distances
TEST(DistTest, ConverseDistEqual) {
    Location prev(0, 0);
    for(int i = 0; i < MAX_RAND_TESTS; i++) {
        Location temp(MIN_LOC_COORD + (rand() % (MAX_LOC_COORD - MIN_LOC_COORD)), 
                     MIN_LOC_COORD + (rand() % (MAX_LOC_COORD - MIN_LOC_COORD)));
        EXPECT_EQ(dist(prev, temp), dist(temp, prev));
        prev = temp;
    }
}