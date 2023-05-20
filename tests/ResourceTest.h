/*
ResourceList tests, Resource tests
Author: Andrew Bergman
12/2/22

This is the unit test for the Resource enum, and the ResourceList class
*/

/////////////////
// Includes
/////////////////

#include <gtest/gtest.h>
#include "../headers/data/ResourceList.h"

/////////////////
// Resource tests
/////////////////

// Quant must be signed because negative quantities represent deficits of materials
TEST(ResourceTest, QuantSigned)
{
    EXPECT_NO_THROW(assert(std::is_signed<Quant>()));
}

// test that COUNT is the last thing in the resource enum
TEST(ResourceTest, ProperCOUNT)
{
    ResourceList rl;
    EXPECT_NO_THROW(assert(Resource(rl.size()) == Resource::COUNT));
}

// test that the prefix operatior++ is working for Resources
TEST(ResourceTest, PrefixOperatorAddition)
{
    Resource a = Resource(0);
    Resource b = ++a;
    EXPECT_EQ(b, Resource(1));
    EXPECT_EQ(a, Resource(1));
}

// test that the postfix operatior++ is working for Resources
TEST(ResourceTest, PostfixOperatorAdditon)
{
    Resource a = Resource(0);
    Resource b = a++;
    EXPECT_EQ(b, Resource(0));
    EXPECT_EQ(a, Resource(1));
}

// test that the prefix operatior-- is working for Resources
TEST(ResourceTest, PrefixOperatorSubtration)
{
    Resource a = Resource(1);
    Resource b = --a;
    EXPECT_EQ(b, Resource(0));
    EXPECT_EQ(a, Resource(0));
}

// test that the postfix operatior-- is working for Resources
TEST(ResourceTest, PostfixOperatorSubtration)
{
    Resource a = Resource(1);
    Resource b = a--;
    EXPECT_EQ(b, Resource(1));
    EXPECT_EQ(a, Resource(0));
}

// test that Resources wraparound correctly for all ==/-- operators
TEST(ResourceTest, PrefixOperatorAdditionWraparound)
{
    Resource a = Resource::COUNT;
    Resource b = ++a;
    EXPECT_EQ(b, Resource(0));
    EXPECT_EQ(a, Resource(0));
}

TEST(ResourceTest, PostfixOperatorAdditionWraparound)
{
    Resource a = Resource::COUNT;
    Resource b = a++;
    EXPECT_EQ(b, Resource::COUNT);
    EXPECT_EQ(a, Resource(0));
}

TEST(ResourceTest, PrefixOperatorSubtractionWraparound)
{
    Resource a = Resource(0);
    Resource b = --a;
    EXPECT_EQ(b, Resource::COUNT);
    EXPECT_EQ(a, Resource::COUNT);
}

TEST(ResourceTest, PostfixOperatorSubtrationWraparound)
{
    Resource a = Resource(0);
    Resource b = a--;
    EXPECT_EQ(b, Resource(0));
    EXPECT_EQ(a, Resource::COUNT);
}

/////////////////
// ResourceList tests
/////////////////

// default constructor
TEST(ResourceListTest, DefaultConstrutor)
{
    ResourceList a;
    for (ResourceList::iterator i = a.begin(); i != a.end(); i++)
    {
        EXPECT_EQ(*i, 0);
    }
}

// constructor with arguments
TEST(ResourceListTest, ArgumentedConstructor)
{
    const int TEST_VALUE = 17;
    ResourceList a = ResourceList(
        {
            {Resource::Copper, TEST_VALUE},
            {Resource::Steel, TEST_VALUE},
            {Resource::Iron, TEST_VALUE},
            {Resource::Stone, TEST_VALUE},
            {Resource::Uranium, TEST_VALUE},
            {Resource::Circuits, TEST_VALUE},
            {Resource::Fish, TEST_VALUE},
            {Resource::Wood, TEST_VALUE},
            {Resource::NuclearFuel, TEST_VALUE},
            {Resource::Wire, TEST_VALUE},
            {Resource::Engine, TEST_VALUE},
            {Resource::Gear, TEST_VALUE}
        }
    );
    for (ResourceList::iterator i = a.begin(); i != a.end(); i++)
    {
        EXPECT_EQ(*i, TEST_VALUE);
    }
}

// test isEmpty
TEST(ResourceListTest, IsEmptyTrue)
{
    ResourceList a;
    EXPECT_TRUE(a.isEmpty());
}

TEST(ResourceListTest, IsEmptyFalse)
{
    const int TEST_VALUE = 17;
    ResourceList a = ResourceList(
        {
            {Resource::Copper, TEST_VALUE},
            {Resource::Steel, TEST_VALUE},
            {Resource::Iron, TEST_VALUE},
            {Resource::Stone, TEST_VALUE},
            {Resource::Uranium, TEST_VALUE},
            {Resource::Circuits, TEST_VALUE},
            {Resource::Fish, TEST_VALUE},
            {Resource::Wood, TEST_VALUE},
            {Resource::NuclearFuel, TEST_VALUE},
            {Resource::Wire, TEST_VALUE},
            {Resource::Engine, TEST_VALUE},
            {Resource::Gear, TEST_VALUE}
        }
    );
    EXPECT_FALSE(a.isEmpty());
}

// test allPositive
TEST(ResourceListTest, AllPositiveTrue)
{
    const int TEST_VALUE = 17;
    ResourceList a = ResourceList(
        {
            {Resource::Copper, TEST_VALUE},
            {Resource::Steel, TEST_VALUE},
            {Resource::Iron, TEST_VALUE},
            {Resource::Stone, TEST_VALUE},
            {Resource::Uranium, TEST_VALUE},
            {Resource::Circuits, TEST_VALUE},
            {Resource::Fish, TEST_VALUE},
            {Resource::Wood, TEST_VALUE},
            {Resource::NuclearFuel, TEST_VALUE},
            {Resource::Wire, TEST_VALUE},
            {Resource::Engine, TEST_VALUE},
            {Resource::Gear, TEST_VALUE}
        }
    );
    EXPECT_TRUE(a.allPositive());
}

TEST(ResourceListTest, AllPositiveFalse)
{
    const int TEST_VALUE = -17;
    ResourceList a = ResourceList(
        {
            {Resource::Copper, TEST_VALUE},
            {Resource::Steel, TEST_VALUE},
            {Resource::Iron, TEST_VALUE},
            {Resource::Stone, TEST_VALUE},
            {Resource::Uranium, TEST_VALUE},
            {Resource::Circuits, TEST_VALUE},
            {Resource::Fish, TEST_VALUE},
            {Resource::Wood, TEST_VALUE},
            {Resource::NuclearFuel, TEST_VALUE},
            {Resource::Wire, TEST_VALUE},
            {Resource::Engine, TEST_VALUE},
            {Resource::Gear, TEST_VALUE}
        }
    );
    EXPECT_FALSE(a.allPositive());
}

// test getNegative
TEST(ResourceListTest, GetNegative)
{
    const int TEST_VALUE = 17;
    ResourceList a = ResourceList(
        {
            {Resource::Copper, -TEST_VALUE}, //-
            {Resource::Steel, TEST_VALUE},
            {Resource::Iron, -TEST_VALUE}, //-
            {Resource::Stone, TEST_VALUE},
            {Resource::Uranium, -TEST_VALUE}, //-
            {Resource::Circuits, TEST_VALUE},
            {Resource::Fish, -TEST_VALUE}, //-
            {Resource::Wood, TEST_VALUE},
            {Resource::NuclearFuel, -TEST_VALUE}, //-
            {Resource::Wire, TEST_VALUE},
            {Resource::Engine, -TEST_VALUE}, //-
            {Resource::Gear, TEST_VALUE}
        }
    );
    EXPECT_EQ(a.getNegative().size(), 6);
}

// test getPositive
TEST(ResourceListTest, GetPositive)
{
    const int TEST_VALUE = 17;
    ResourceList a(
        {
            {Resource::Copper, -TEST_VALUE}, //-
            {Resource::Steel, TEST_VALUE},
            {Resource::Iron, -TEST_VALUE}, //-
            {Resource::Stone, TEST_VALUE},
            {Resource::Uranium, -TEST_VALUE}, //-
            {Resource::Circuits, TEST_VALUE},
            {Resource::Fish, -TEST_VALUE}, //-
            {Resource::Wood, TEST_VALUE},
            {Resource::NuclearFuel, -TEST_VALUE}, //-
            {Resource::Wire, TEST_VALUE},
            {Resource::Engine, -TEST_VALUE}, //-
            {Resource::Gear, TEST_VALUE}
        }
    );
    EXPECT_EQ(a.getPositive().size(), 6);
}

// test getNegative
TEST(ResourceListTest, GetNegativeList)
{
    const int TEST_VALUE = 17;
    ResourceList a(
        {
            {Resource::Copper, -TEST_VALUE}, //-
            {Resource::Steel, TEST_VALUE},
            {Resource::Iron, -TEST_VALUE}, //-
            {Resource::Stone, TEST_VALUE},
            {Resource::Uranium, -TEST_VALUE}, //-
            {Resource::Circuits, TEST_VALUE},
            {Resource::Fish, -TEST_VALUE}, //-
            {Resource::Wood, TEST_VALUE},
            {Resource::NuclearFuel, -TEST_VALUE}, //-
            {Resource::Wire, TEST_VALUE},
            {Resource::Engine, -TEST_VALUE}, //-
            {Resource::Gear, TEST_VALUE}
        }
    ), b(
        {
            {Resource::Copper, -TEST_VALUE}, //-
            {Resource::Iron, -TEST_VALUE}, //-
            {Resource::Uranium, -TEST_VALUE}, //-
            {Resource::Fish, -TEST_VALUE}, //-
            {Resource::NuclearFuel, -TEST_VALUE}, //-
            {Resource::Engine, -TEST_VALUE}, //-
        }
    );
    EXPECT_EQ(a.getNegativeList(), b);
}

// test getPositive
TEST(ResourceListTest, GetPositiveList)
{
    const int TEST_VALUE = 17;
    ResourceList a(
        {
            {Resource::Copper, -TEST_VALUE}, //-
            {Resource::Steel, TEST_VALUE},
            {Resource::Iron, -TEST_VALUE}, //-
            {Resource::Stone, TEST_VALUE},
            {Resource::Uranium, -TEST_VALUE}, //-
            {Resource::Circuits, TEST_VALUE},
            {Resource::Fish, -TEST_VALUE}, //-
            {Resource::Wood, TEST_VALUE},
            {Resource::NuclearFuel, -TEST_VALUE}, //-
            {Resource::Wire, TEST_VALUE},
            {Resource::Engine, -TEST_VALUE}, //-
            {Resource::Gear, TEST_VALUE}
        }
    ), b(
        {
            {Resource::Steel, TEST_VALUE},
            {Resource::Stone, TEST_VALUE},
            {Resource::Circuits, TEST_VALUE},
            {Resource::Wood, TEST_VALUE},
            {Resource::Wire, TEST_VALUE},
            {Resource::Gear, TEST_VALUE}
        }
    );
    EXPECT_EQ(a.getPositiveList(), b);
}

// test operator==
TEST(ResourceListTest, EqualOperatorTrue)
{
    const int TEST_VALUE = 17;
    ResourceList a = ResourceList(
        {
            {Resource::Copper, -TEST_VALUE}, //-
            {Resource::Steel, TEST_VALUE}
        }
    );
    ResourceList b = ResourceList(
        {
            {Resource::Copper, -TEST_VALUE}, //-
            {Resource::Steel, TEST_VALUE}
        }
    );
    EXPECT_EQ(a[0], b[0]);
    EXPECT_EQ(a[1], b[1]);
    EXPECT_TRUE(a == b);
}

TEST(ResourceListTest, EqualOperatorFalse)
{
    const int TEST_VALUE = 17;
    ResourceList a = ResourceList(
        {
            {Resource::Copper, TEST_VALUE}, //-
            {Resource::Steel, TEST_VALUE}
        }
    );
    ResourceList b = ResourceList(
        {
            {Resource::Copper, -TEST_VALUE}, //-
            {Resource::Steel, TEST_VALUE}
        }
    );
    EXPECT_NE(a[0], b[0]);
    EXPECT_EQ(a[1], b[1]);
    EXPECT_FALSE(a == b);
}

// test operator+
TEST(ResourceListTest, AddTwoLists)
{
    const int TEST_VALUE = 17;
    ResourceList a = ResourceList(
        {
            {Resource::Copper, -TEST_VALUE},
            {Resource::Steel, -TEST_VALUE},
            {Resource::Iron, TEST_VALUE},
            {Resource::Stone, TEST_VALUE}

        }
    );
    ResourceList b = ResourceList(
        {
            {Resource::Copper, -TEST_VALUE},
            {Resource::Steel, TEST_VALUE},
            {Resource::Iron, -TEST_VALUE},
            {Resource::Stone, TEST_VALUE}
        }
    );
    ResourceList a_copy = a;
    ResourceList b_copy = b;
    ResourceList c = a + b;
    EXPECT_EQ(c[0], -2*TEST_VALUE);
    EXPECT_EQ(c[1], 0);
    EXPECT_EQ(c[2], 0);
    EXPECT_EQ(c[3], 2*TEST_VALUE);
    EXPECT_EQ(a, a_copy); // verify a was not changed
    EXPECT_EQ(b, b_copy); // verify b was not changed
}

// test operator-
TEST(ResourceListTest, SubtractTwoLists)
{
    const int TEST_VALUE = 17;
    ResourceList a = ResourceList( // [-17,-17,17,17...]
        {
            {Resource::Copper, -TEST_VALUE},
            {Resource::Steel, -TEST_VALUE},
            {Resource::Iron, TEST_VALUE},
            {Resource::Stone, TEST_VALUE}
        }
    );
    ResourceList b = ResourceList( // [-17,17,-17,17...]
        {
            {Resource::Copper, -TEST_VALUE},
            {Resource::Steel, TEST_VALUE},
            {Resource::Iron, -TEST_VALUE},
            {Resource::Stone, TEST_VALUE}
        }
    );
    ResourceList a_copy = a;
    ResourceList b_copy = b;
    ResourceList c = a - b;
    EXPECT_EQ(c[0], 0);
    EXPECT_EQ(c[1], -2 * TEST_VALUE);
    EXPECT_EQ(c[2], 2 * TEST_VALUE);
    EXPECT_EQ(c[3], 0);
    EXPECT_EQ(a, a_copy); // verify a was not changed
    EXPECT_EQ(b, b_copy); // verify b was not changed
}

// test operator +=
TEST(ResourceListTest, PlusEqualOperator)
{
    const int TEST_VALUE = 17;
    ResourceList a = ResourceList( // [-17,-17,17,17...]
        {
            {Resource::Copper, TEST_VALUE},
        }
    );
    ResourceList b = ResourceList( // [-17,17,-17,17...]
        {
            {Resource::Copper, TEST_VALUE},
        }
    );
    ResourceList a_copy = a;
    ResourceList b_copy = b;
    a += b;
    EXPECT_EQ(a[0], 2 * TEST_VALUE);
    EXPECT_NE(a, a_copy); // verify a was changed
    EXPECT_EQ(b, b_copy); // verify b was not changed
}

// test operator -=
TEST(ResourceListTest, MinusEqualOperator)
{
    const int TEST_VALUE = 17;
    ResourceList a = ResourceList( // [-17,-17,17,17...]
        {
            {Resource::Copper, TEST_VALUE},
        }
    );
    ResourceList b = ResourceList( // [-17,17,-17,17...]
        {
            {Resource::Copper, TEST_VALUE},
        }
    );
    ResourceList a_copy = a;
    ResourceList b_copy = b;
    a -= b;
    EXPECT_EQ(a[0], 0);
    EXPECT_NE(a, a_copy); // verify a was changed
    EXPECT_EQ(b, b_copy); // verify b was not changed
}
