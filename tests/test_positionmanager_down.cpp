#include "catch.hpp"
#include "../src/PositionManager_Down.h"

using namespace std;

// box is moving down. If Current position is Position(a, b), then 
//      first position is Position(a, b+1). Corresponding to moving down.
//      second position is Position(a-1, b+1).
//      third position is Position(a+1, b+1).
TEST_CASE("Should return next positions, which are down, then the diagonal position down and right, then the diagonal position down and left.")
{
    // note0's type is 4, which means it arrives at Position(5, 5);
    BoxNote note0{4, Position{5, 5}, Position{5, 4}, std::chrono::high_resolution_clock::now()};
    Box box{0, 10, 10};
    box.addNote(note0);

    PositionManager_Down downPositionMover{Position{10, 10}};
    vector<Position> positions = downPositionMover.getFuturePositions(box);
    REQUIRE(positions[0] == Position{5, 6});
    REQUIRE(positions[1] == Position{4, 6});
    REQUIRE(positions[2] == Position{6, 6});

}

TEST_CASE(" PositionManager_Down::atEnd() returns true if box has reached its final position.")
{
    // box has arrived at Position{1, 1}.    
    BoxNote note0{4, Position{1, 1}, Position{1, 1}, std::chrono::high_resolution_clock::now()};
    Box box{0, 10, 10};
    box.addNote(note0);
    PositionManager_Down downPositionManager(Position{1, 1});
    REQUIRE(true == downPositionManager.atEnd(box));
}

TEST_CASE(" PositionManager_Down::atEnd() returns false if box is not at its final position.")
{
    // box has arrived at Position{0, 0}.    
    BoxNote note0{4, Position{0, 0}, Position{0, 0}, std::chrono::high_resolution_clock::now()};
    Box box{0, 10, 10};
    box.addNote(note0);
    PositionManager_Down downPositionManager(Position{1, 1});
    REQUIRE(false == downPositionManager.atEnd(box));
}
