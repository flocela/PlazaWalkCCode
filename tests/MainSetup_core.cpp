#include "catch.hpp"
#include "../src/MainSetup.h"

using namespace std;

TEST_CASE("MainSetup_core::")
{
    SECTION("addGroupsOfBoxes() adds one group of boxes.")
    {
        vector<Box> boxes{};
        MainSetup::addGroupsOfBoxes(
            boxes,
            0,
            10,
            1,
            100);
        REQUIRE(100 == boxes.size());
        REQUIRE(0 == boxes[0].getId());
        REQUIRE(99 == boxes[99].getId());
        REQUIRE(10 == boxes[0].getGroupId());
        REQUIRE(10 == boxes[99].getGroupId());
    }
    SECTION("addGroupsOfBoxes() adds one group and then another.")
    {
        vector<Box> boxes{};
        MainSetup::addGroupsOfBoxes(
            boxes,
            0,
            10,
            1,
            100);
        MainSetup::addGroupsOfBoxes(
            boxes,
            100,
            20,
            1,
            100);
            
        REQUIRE(200 == boxes.size());
        REQUIRE(0 == boxes[0].getId());
        REQUIRE(100 == boxes[100].getId());
        REQUIRE(10 == boxes[0].getGroupId());
        REQUIRE(20 == boxes[100].getGroupId());
    }
    SECTION("addGroupsOfBoxes() adds one group and then two groups")
    {
        vector<Box> boxes{};
        MainSetup::addGroupsOfBoxes(
            boxes,
            0,
            0,
            1,
            100);
        MainSetup::addGroupsOfBoxes(
            boxes,
            100,
            1,
            2,
            100);
            
        REQUIRE(300 == boxes.size());
        REQUIRE(0 == boxes[0].getId());
        REQUIRE(100 == boxes[100].getId());
        REQUIRE(200 == boxes[200].getId());
        REQUIRE(0 == boxes[0].getGroupId());
        REQUIRE(1 == boxes[100].getGroupId());
        REQUIRE(2 == boxes[200].getGroupId());
    }
    SECTION("addAGroupOfBoxes() adds one group.")
    {
        vector<Box> boxes{};
        MainSetup::addAGroupOfBoxes(
            boxes,
            0,
            10,
            100);
        REQUIRE(100 == boxes.size());
        REQUIRE(0 == boxes[0].getId());
        REQUIRE(99 == boxes[99].getId());
        REQUIRE(10 == boxes[0].getGroupId());
        REQUIRE(10 == boxes[99].getGroupId());
    }
    SECTION("addAGroupOfBoxes() adds one group and then another.")
    {
        vector<Box> boxes{};
        MainSetup::addAGroupOfBoxes(
            boxes,
            0,
            10,
            100);
        MainSetup::addAGroupOfBoxes(
            boxes,
            100,
            20,
            100);
            
        REQUIRE(200 == boxes.size());
        REQUIRE(0 == boxes[0].getId());
        REQUIRE(100 == boxes[100].getId());
        REQUIRE(10 == boxes[0].getGroupId());
        REQUIRE(20 == boxes[100].getGroupId());
    }

    SECTION("deleteRect() deletes rectangle from vector.")
    {
        vector<pair<Position, Position>> rectangles{};
        rectangles.push_back({Position{0, 0}, Position{10, 10}}); 
        rectangles.push_back({Position{10, 10}, Position{20, 20}}); 
        rectangles.push_back({Position{20, 20}, Position{30, 30}}); 
        rectangles.push_back({Position{30, 30}, Position{40, 40}}); 

        
        vector<pair<Position, Position>> reducedRectangles{};
        reducedRectangles.push_back({Position{0, 0}, Position{10, 10}}); 
        reducedRectangles.push_back({Position{10, 10}, Position{20, 20}}); 
        reducedRectangles.push_back({Position{30, 30}, Position{40, 40}}); 

        REQUIRE(reducedRectangles ==
                      MainSetup::deleteRect(rectangles, {Position{20, 20}, Position{30, 30}}) );
    }
        
}