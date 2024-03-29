#include "catch.hpp"

#include "../src/BoardNote.h"

using namespace std;

TEST_CASE("BoardNote returns type")
{
    BoardNote note{22, SpotType::to_leave};
    REQUIRE(SpotType::to_leave == note.getType());
}

TEST_CASE("BoardNote returns boxId")
{
    BoardNote note{22, SpotType::arrive};
    REQUIRE(22 == note.getBoxId());
}

TEST_CASE("BoardNote == operator returns true")
{
    BoardNote note1{22, SpotType::left};
    BoardNote note2{22, SpotType::left}; 
    REQUIRE(note1 == note2);
}

TEST_CASE("BoardNote == operator returns false if boxId's are different")
{
    BoardNote note1{22, SpotType::left};
    BoardNote note2{24, SpotType::left}; 
    REQUIRE_FALSE(note1 == note2);
}

TEST_CASE("BoardNote == operator returns false if type's are different")
{
    BoardNote note1{22, SpotType::left};
    BoardNote note2{22, SpotType::to_arrive}; 
    REQUIRE_FALSE(note1 == note2);
}
