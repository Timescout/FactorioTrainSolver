#include <gtest/gtest.h>
#include "../headers/data/Factory.h"

// test constructor from coordinates
TEST(FactoryTest, DefaultConstructor)
{
    Factory factory;
    EXPECT_EQ(factory.getLoc(), Location(0,0));
    EXPECT_EQ(factory.getBaseQuants(), ResourceList());
    EXPECT_EQ(factory.getUnallocated(), ResourceList());
    EXPECT_EQ(factory.getAllocated(), ResourceList());
}

TEST(FactoryTest, CoordConstructorTest)
{
    ResourceList iron(
        {
            {Resource::Iron, 100}
        });
    Factory factory(3,4, iron);
    Location place(3,4);
    EXPECT_EQ(factory.getLoc(), place);
    EXPECT_EQ(factory.getBaseQuants(), iron);
    EXPECT_EQ(factory.getUnallocated(), iron);
}

TEST(FactoryTest, LocationConstructorTest)
{
        ResourceList iron(
        {
            {Resource::Iron, 100}
        });
    Location place(3,4);
    Factory factory(place, iron);
    EXPECT_EQ(factory.getLoc(), place);
    EXPECT_EQ(factory.getBaseQuants(), iron);
    EXPECT_EQ(factory.getUnallocated(), iron);
}

// test factory Factory function
TEST(FactoryTest, JunctionCreation)
{
    const Coord x = 5, y = 17;
    Location place(x, y);
    Factory juction_a = Factory::makeJunction(x, y);
    Factory juction_b = Factory::makeJunction(place);
    EXPECT_EQ(juction_a.getLoc(), juction_b.getLoc());
    EXPECT_EQ(juction_a.getLoc(), place);
    EXPECT_EQ(place, juction_b.getLoc());
}

// test getter functions
TEST(FactoryTest, GetterTest) {
    ResourceList rl((std::map<Resource, Quant>){
        {Resource::Copper, 1},
        {Resource::Steel, -1}
    });
    Factory f(Location(0, 0), rl);
    EXPECT_EQ(f.getLoc(), Location(0, 0));
    EXPECT_EQ(f.getBaseQuants(), rl);
    EXPECT_EQ(f.getUnallocated(), rl);
    EXPECT_EQ(f.getAllocated(), ResourceList());
}

// attempt to over-produce and over-consume
TEST(FactoryTest, OverallocNormal) {
    ResourceList rl((std::map<Resource, Quant>){
        {Resource::Copper, 1},
        {Resource::Steel, -1}
    }), one_prod((std::map<Resource, Quant>){
        {Resource::Copper, 1}
    }), one_cons((std::map<Resource, Quant>){
        {Resource::Steel, -1}
    });
    Factory f(Location(0, 0), rl);
    // get base quants, allocs
    EXPECT_EQ(f.getBaseQuants(), rl);
    EXPECT_EQ(f.getUnallocated(), rl);
    EXPECT_EQ(f.getAllocated(), ResourceList());

    // successful prod alloc
    EXPECT_TRUE(f.allocate(one_prod));
    // get new allocs
    EXPECT_EQ(f.getUnallocated(), one_cons);
    EXPECT_EQ(f.getAllocated(), one_prod);

    // unsuccessful prod alloc
    EXPECT_FALSE(f.allocate(one_prod));
    // get same allocs
    EXPECT_EQ(f.getUnallocated(), one_cons);
    EXPECT_EQ(f.getAllocated(), one_prod);

    // successful cons alloc
    EXPECT_TRUE(f.allocate(one_cons));
    // get new allocs
    EXPECT_EQ(f.getUnallocated(), ResourceList());
    EXPECT_EQ(f.getAllocated(), rl);

    // unsuccessful cons alloc
    EXPECT_FALSE(f.allocate(one_cons));
    // get same allocs
    EXPECT_EQ(f.getUnallocated(), ResourceList());
    EXPECT_EQ(f.getAllocated(), rl);
}

// attempt to reverse produce and reverse consume
TEST(FactoryTest, OverallocReverse) {
    ResourceList rl((std::map<Resource, Quant>){
        {Resource::Copper, 1},
        {Resource::Steel, -1}
    }), one_rev_prod((std::map<Resource, Quant>){
        {Resource::Steel, 1}
    }), one_rev_cons((std::map<Resource, Quant>){
        {Resource::Copper, -1}
    });
    Factory f(Location(0, 0), rl);
    // get base quants, allocs
    EXPECT_EQ(f.getBaseQuants(), rl);
    EXPECT_EQ(f.getUnallocated(), rl);
    EXPECT_EQ(f.getAllocated(), ResourceList());

    // unsuccessful reverse prod alloc
    EXPECT_FALSE(f.allocate(one_rev_prod));
    // get same allocs
    EXPECT_EQ(f.getUnallocated(), rl);
    EXPECT_EQ(f.getAllocated(), ResourceList());

    // unsuccessful reverse cons alloc
    EXPECT_FALSE(f.allocate(one_rev_cons));
    // get same allocs
    EXPECT_EQ(f.getUnallocated(), rl);
    EXPECT_EQ(f.getAllocated(), ResourceList());
}

TEST(FactoryTest, ResetTest)
{
    ResourceList rl((std::map<Resource, Quant>){
        {Resource::Copper, 1}
    }), one_request((std::map<Resource, Quant>){
        {Resource::Copper, 1}
    });

    Factory factory(0, 0, rl);
    factory.allocate(one_request);
    factory.resetAllocated();
    EXPECT_EQ(factory.getUnallocated(), factory.getBaseQuants());
}

TEST(FactoryTest, deallocate)
{
    ResourceList rl(
        {
            {Resource::Copper, 10},
            {Resource::Iron, -10}
        });
    Factory factory(0,0,rl);
    EXPECT_TRUE(factory.allocate(rl));
    EXPECT_EQ(factory.getUnallocated(), ResourceList());
    EXPECT_EQ(factory.getAllocated(), rl);
    EXPECT_TRUE(factory.deallocate(rl));
    EXPECT_EQ(factory.getUnallocated(), rl);
    EXPECT_EQ(factory.getAllocated(), ResourceList());
}