#include <gtest/gtest.h>
#include "../headers/data/Location.h"

// numerical values to use througout testing location
const int zero = 0;
const int large_positive = 9999;
const int large_negative = -9999;
const int small_positive = 1;
const int small_negative = -1;

// points used for testing addition and subtraction
Location origin(zero,zero);
Location positive(small_positive,small_positive);
Location negative(small_negative,small_negative);


// check that Coord type is signed
TEST(LocationTest, CoordSigned) {
    EXPECT_NO_THROW(assert(std::is_signed<Coord>()));
}

// test constructor with given coordinate values
TEST(LocationTest, ConstructorTest) {
    Location loc(zero, zero);
    EXPECT_EQ(loc.x, zero);
    EXPECT_EQ(loc.y, zero);
}

// test copy constructor
TEST(LocationTest, CopyConstructor) {
    Location loc(large_positive, large_positive);
    Location loc_copy(loc);
    EXPECT_EQ(loc_copy.x, large_positive);
    EXPECT_EQ(loc_copy.y, large_positive);
}

// test assignment operator
TEST(LocationTest, AssignmentOperator) {
    Location loc(large_negative, large_negative);
    Location loc_assign = loc;
    EXPECT_EQ(loc_assign.x, large_negative);
    EXPECT_EQ(loc_assign.y, large_negative);
}

// test location addition operator
TEST(LocationTest, AdditionTest) {
    Location double_pos = positive + positive;
    EXPECT_EQ(double_pos.x, 2*small_positive);
    EXPECT_EQ(double_pos.y, 2*small_positive);

    Location double_neg = negative + negative;
    EXPECT_EQ(double_neg.x, 2*small_negative);
    EXPECT_EQ(double_neg.y, 2*small_negative);
    
    Location calc_origin = positive + negative;
    EXPECT_EQ(calc_origin.x, origin.x);
    EXPECT_EQ(calc_origin.y, origin.y);
    
    Location calc_pos = positive + origin;
    EXPECT_EQ(calc_pos.x, positive.x);
    EXPECT_EQ(calc_pos.y, positive.y);
    
    Location calc_neg = origin + negative;
    EXPECT_EQ(calc_neg.x, negative.x);
    EXPECT_EQ(calc_neg.y, negative.y);
}

// test location subtraction operator
TEST(LocationTest, SubtractionTest) {
    Location origin_pos = positive - positive;
    EXPECT_EQ(origin_pos.x, origin.x);
    EXPECT_EQ(origin_pos.y, origin.y);

    Location origin_neg = negative - negative;
    EXPECT_EQ(origin_neg.x, origin.x);
    EXPECT_EQ(origin_neg.y, origin.y);

    Location sub_neg = origin - negative;
    EXPECT_EQ(sub_neg.x, positive.x);
    EXPECT_EQ(sub_neg.y, positive.y);

    Location sub_pos = origin - positive;
    EXPECT_EQ(sub_pos.x, negative.x);
    EXPECT_EQ(sub_pos.y, negative.y);
}

// test Location hash, 
TEST(LocationTest, HashTest)
{
    Location WA(10,10);
    Location OR(-9,-17);
    Location ID(-15, 8);
    LocationHasher hasher;
    EXPECT_NE(hasher(WA), hasher(OR));
    EXPECT_NE(hasher(WA), hasher(ID));
    EXPECT_NE(hasher(OR), hasher(ID));
}

// test Location comparison
TEST(LocationTest, LessThan)
{
    Location a(large_positive,large_positive);
    Location b(zero, zero);
    EXPECT_TRUE(b < a);
    EXPECT_FALSE(a < b);
}

TEST(LocationTest, LessThanSameX)
{
    Location a(large_positive,large_positive);
    Location b(large_positive, zero);
    EXPECT_TRUE(b < a);
    EXPECT_FALSE(a < b);
}

TEST(LocationTest, GreaterThan)
{
    Location a(large_positive,large_positive);
    Location b(zero, zero);
    EXPECT_FALSE(b > a);
    EXPECT_TRUE(a > b);
}

TEST(LocationTest, GreaterThanSameX)
{
    Location a(large_positive,large_positive);
    Location b(large_positive, zero);
    EXPECT_FALSE(b > a);
    EXPECT_TRUE(a > b);
}

TEST(LocationTest, LessThanEqual)
{
    Location a(large_positive,large_positive);
    Location b(zero, zero);
    EXPECT_TRUE(b <= a);
    EXPECT_FALSE(a <= b);
    EXPECT_TRUE(a <= a);
}

TEST(LocationTest, LessThanEqualSameX)
{
    Location a(large_positive,large_positive);
    Location b(large_positive, zero);
    EXPECT_TRUE(b <= a);
    EXPECT_FALSE(a <= b);
    EXPECT_TRUE(a <= a);
}

TEST(LocationTest, GreaterThanEqual)
{
    Location a(large_positive,large_positive);
    Location b(zero, zero);
    EXPECT_FALSE(b >= a);
    EXPECT_TRUE(a >= b);
    EXPECT_TRUE(a >= a);
}

TEST(LocationTest, GreaterThanEqualSameX)
{
    Location a(large_positive,large_positive);
    Location b(large_positive, zero);
    EXPECT_FALSE(b >= a);
    EXPECT_TRUE(a >= b);
    EXPECT_TRUE(a >= a);
}

TEST(LocationTest, LocationCompareTest)
{
    Location a(large_positive,large_positive);
    Location b(large_positive, zero);
    LocationCompare comp;
    EXPECT_EQ(comp(a,b), a < b);
}

// TEST(DistTest, Constructor) // for if we ever create a constructor for distances
// {
//     const Coord VALUE = 17;
//     Dist a(VALUE,VALUE);
//     EXPECT_EQ(a.rat_, VALUE);
//     EXPECT_EQ(a.irrat_, VALUE);

// }