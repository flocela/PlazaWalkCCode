#include "catch.hpp"
#include <iostream>
#include "../src/Spot.h"
#include <regex>
#include <thread>

using namespace std;

TEST_CASE("Spot:: Copy Constructor")
{
    Position pos{3, 4};
    Spot spot{pos};
    spot.changeNote(BoardNote{10, SpotType::to_arrive});

    Spot spotCopy = spot;
    
    REQUIRE(spot.getPosition() == spotCopy.getPosition());
    REQUIRE(spot.getBoardNote() == spotCopy.getBoardNote());
}

TEST_CASE("Spot:: Move Constructor")
{
    Position pos{3, 4};
    Spot spot{pos};
    spot.changeNote(BoardNote{10, SpotType::to_arrive});

    Spot spot2(std::move(spot));
 
    REQUIRE(spot2.getPosition() == spot.getPosition());
    REQUIRE(spot2.getBoardNote() == spot.getBoardNote());
}

TEST_CASE("Spot:: Spot is constructed and should be in a default state.")
{
    // Construct Spot.
    Position pos{3, 4};
    Spot spot{pos};

    SECTION("Returns the Position given in the constructor.")
    {
        REQUIRE(pos == spot.getPosition());
    }
    
    SECTION("Returns the default type and default boxId.")
    {
        REQUIRE(-1 == spot.getBoardNote().getBoxId());
        REQUIRE(SpotType::left == spot.getBoardNote().getType());
    }
}

TEST_CASE("Spot:: Spot originally does not hold a box)")
{
    Spot spot{Position{100, 200}};

    SECTION("and receives an 'Imminent Arrival' note, then spot will contain the new box_id and type 2.")
    {
        // Box with boxId 10 is about to arrive.
        // 'Imminent Arrival' type is 2.
        BoardNote imminentArrivalNote{10, SpotType::to_arrive};

        Spot spot{Position{8, 9}};
        spot.changeNote(imminentArrivalNote);
        
        REQUIRE(10 == spot.getBoardNote().getBoxId());
        REQUIRE(SpotType::to_arrive == spot.getBoardNote().getType());
    }
   
    SECTION("and receives an 'Arrival' note, then an exception is thrown. Spot's type must be 'Imminent Arrival' type to accept an 'Arrival' note.")
    {   
        // Box with boxId 10 arrives.
        // 'Arrives' type is 4.
        BoardNote arrivalNote{10, SpotType::arrive};

        REQUIRE_THROWS(spot.changeNote(arrivalNote));
        REQUIRE(-1 == spot.getBoardNote().getBoxId());
        REQUIRE(SpotType::left == spot.getBoardNote().getType());
    }
    
    SECTION("and receives an 'Imminent Departure' note, then an exception is thrown. Spot's type must be'Arrived' type to accept an 'Imminent Departure' note.")
    {
        // Box with boxId 10 is about to leave.
        // 'Imminent Departure' type is 1.
        BoardNote imminentDepartureNote{10, SpotType::to_leave};

        REQUIRE_THROWS(spot.changeNote(imminentDepartureNote));
        REQUIRE(-1 == spot.getBoardNote().getBoxId());
        REQUIRE(SpotType::left == spot.getBoardNote().getType());
    }
    
    SECTION("and recieves a 'Departure' note, then an exception is thrown. Spot's type must be 'Imminent Departure' type before accepting a 'Departure' note.")
    {
        // Box with boxId 10 leaves.
        // 'Departure' type is 3.
        BoardNote departureNote{10, SpotType::left};

        REQUIRE_THROWS(spot.changeNote(departureNote));
        REQUIRE(-1 == spot.getBoardNote().getBoxId());
        REQUIRE(SpotType::left == spot.getBoardNote().getType());
    }
}

TEST_CASE("Spot:: Spot originally has the type 'Imminent Departure'")
{
    // Box with boxId 10 is about to leave.
    // 'Imminently Departure' type is 1.
    BoardNote imminentArrivalNote{10, SpotType::to_arrive};
    BoardNote arrivalNote{10, SpotType::arrive};
    BoardNote imminentDepartureNote{10, SpotType::to_leave};
    Spot spot{Position{100, 200}};
    spot.changeNote(imminentArrivalNote);
    spot.changeNote(arrivalNote);
    spot.changeNote(imminentDepartureNote);

    SECTION("and receives a note with the with the same boxId")
    {
        SECTION("and an 'Imminent Departure' type, then an exception is thrown. The Spot's type is already 'Imminent Departure.")
        {
            // Box with boxId 10 is about to leave.
            // 'Imminent Departure' type is 1.
            BoardNote imminentDepartureNote{10, SpotType::to_leave};
            REQUIRE_THROWS(spot.changeNote(imminentDepartureNote));
            REQUIRE(10 == spot.getBoardNote().getBoxId());
            REQUIRE(SpotType::to_leave == spot.getBoardNote().getType());
        }
        
        SECTION("and an 'Imminent Arrival' type, then an exception is thrown. The Spot's type must be The box can not be about to arrive, it has already arrived and is about to leave.")
        {
            // Box with boxId 10 is about to arrive.
            // 'Imminent Arrival' type is 2.
            BoardNote imminentArrivalNote{10, SpotType::to_arrive};

            REQUIRE_THROWS(spot.changeNote(imminentArrivalNote));
            REQUIRE(10 == spot.getBoardNote().getBoxId());
            REQUIRE(SpotType::to_leave == spot.getBoardNote().getType());
        }

        SECTION("and a 'Departure' type, then the Spot's boxId and type will change to -1 and -1.")
        {
            // Box with boxId 10 leaves.
            // 'Departure' type is 3.
            BoardNote departureNote{10, SpotType::left};

            REQUIRE(true == spot.changeNote(departureNote).second);
            REQUIRE(-1 == spot.getBoardNote().getBoxId());
            REQUIRE(SpotType::left == spot.getBoardNote().getType());
        }
        
        SECTION("and an 'Arrival' type, then an exception is thrown. The Spot's type must be 'Imminent Arrival' to accept an 'Arrival' note.")
        {
            // Box with boxId 10 arrives.
            // 'Arrival' type is 4.
            BoardNote arrivalNote{10, SpotType::arrive};

            REQUIRE_THROWS(spot.changeNote(arrivalNote));
            REQUIRE(10 == spot.getBoardNote().getBoxId());
            REQUIRE(SpotType::to_leave == spot.getBoardNote().getType());
        }
    }

    SECTION("and receives a note with the with a different boxId")
    {
        SECTION("and an 'Imminent Departure' type, then an exception is thrown. Spot's type must be 'Arrived' to accept an 'Imminent Departure' note.")
        {
            // Box with boxId 99 is about to leave.
            // 'Imminent Departure' type is 1.
            BoardNote imminentDepartureNote{99, SpotType::to_leave};

            REQUIRE_THROWS(spot.changeNote(imminentDepartureNote));
            REQUIRE(10 == spot.getBoardNote().getBoxId());
            REQUIRE(SpotType::to_leave == spot.getBoardNote().getType());
        }
        
        SECTION("and an 'Imminent Arrival' type, then false is returned, because the Spot currently has the original box, and two boxes can not be at the same spot.")
        {
            // Box with boxId 99 is about to arrive.
            // 'Imminent Arrival' type is 2.
            BoardNote imminentArrivalNote{99, SpotType::to_arrive};

            REQUIRE_FALSE(spot.changeNote(imminentArrivalNote).second);
            REQUIRE(10 == spot.getBoardNote().getBoxId());
            REQUIRE(SpotType::to_leave == spot.getBoardNote().getType());
        }

        SECTION("and a 'Departure' type, then an exception is thrown. Spot's type must be 'ImminentDeparture' to accept an 'Departure' note.")
        {
            // Box with boxId 99 leaves.
            // 'Departure' type is 3.
            BoardNote departureNote{99, SpotType::left};

            REQUIRE_THROWS(spot.changeNote(departureNote));
            REQUIRE(10 == spot.getBoardNote().getBoxId());
            REQUIRE(SpotType::to_leave == spot.getBoardNote().getType());
        }
        
        SECTION("and an 'Arrival' type, then an exception is thrown. Spot's type must be 'Imminent Arrival' to accept an 'Arrival' note.")
        {
            // Box with boxId 99 arrives.
            // 'Arrives' type is 4.
            BoardNote arrivalNote{99, SpotType::arrive};

            REQUIRE_THROWS(spot.changeNote(arrivalNote));
            REQUIRE(10 == spot.getBoardNote().getBoxId());
            REQUIRE(SpotType::to_leave == spot.getBoardNote().getType());
        }
    }
}

TEST_CASE("Spot:: Spot originally has the type 'Imminent Arrival'")
{
    // Box with boxId 10 is about to arrive.
    // 'Imminent Arrival' type is 2.
    BoardNote imminentArrivalNote{10, SpotType::to_arrive};
    Spot spot{Position{100, 200}};
    spot.changeNote(imminentArrivalNote);

    SECTION("and receives a note with the same boxId")
    {
        SECTION("and 'Imminent Departure' type, then an exception is thrown. Spot's type must be 'Arrival' to accept an 'Imminent Departure' note.")
        {
            // Box with boxId 10 is about to leave.
            // 'Imminent Departure' type is 1.
            BoardNote imminentDepartureNote{10, SpotType::to_leave};

            REQUIRE_THROWS(spot.changeNote(imminentDepartureNote));
            
            REQUIRE(10 == spot.getBoardNote().getBoxId());
            REQUIRE(SpotType::to_arrive == spot.getBoardNote().getType());
        }
        
        SECTION("and 'Imminent Arrival' type, then an exception is thrown because the box is already about to leave.")
        {
            // Box with boxId 10 is about to arrive.
            // 'Imminent Arrival' type is 2.
            BoardNote imminentArrivalNote{10, SpotType::to_arrive};

            REQUIRE_THROWS(spot.changeNote(imminentArrivalNote));
            REQUIRE(10 == spot.getBoardNote().getBoxId());
            REQUIRE(SpotType::to_arrive == spot.getBoardNote().getType());
        }

        SECTION("and 'Departure' type, then an exception is thrown. Spot's type must be 'Imminent Departure' note to accept a 'Departure' note.")
        {
            // Box with boxId 10 leaves.
            // 'Departure' type is 3.
            BoardNote departureNote{10, SpotType::left};

            REQUIRE_THROWS(spot.changeNote(departureNote));
            REQUIRE(10 == spot.getBoardNote().getBoxId());
            REQUIRE(SpotType::to_arrive == spot.getBoardNote().getType());
        }
        
        SECTION("and 'Arrival' type, then Spot will have type 4.")
        {
            // Box with boxId 10 arrives.
            // 'Arrives' type is 4.
            BoardNote arrivalNote{10, SpotType::arrive};

            REQUIRE(true == spot.changeNote(arrivalNote).second);
            REQUIRE(10 == spot.getBoardNote().getBoxId());
            REQUIRE(SpotType::arrive == spot.getBoardNote().getType());
        }

    }
    
    SECTION("and receives a note with a different box (different boxId)")
    {
        SECTION("and 'ImminentDeparture' type, then an exception is thrown. Spot must receive an 'Arrival note before it can accept an 'Imminent Departure' note.")
        {
            // Box with boxId 99 is about to leave.
            // 'Imminent Departure' type is 1.
            BoardNote imminentDepartureNote{99, SpotType::to_leave};

            REQUIRE_THROWS(spot.changeNote(imminentDepartureNote));
            REQUIRE(10 == spot.getBoardNote().getBoxId());
            REQUIRE(SpotType::to_arrive == spot.getBoardNote().getType());
        }
        
        SECTION("and 'Imminent Arrival' type, then false is returned. The Spot already contains a box.")
        {
            // Box with boxId 99 is about to arrive.
            // 'Imminent Arrival' type is 2.
            BoardNote imminentArrivalNote{99, SpotType::to_arrive};

            REQUIRE_FALSE(spot.changeNote(imminentArrivalNote).second);
            REQUIRE(10 == spot.getBoardNote().getBoxId());
            REQUIRE(SpotType::to_arrive == spot.getBoardNote().getType());
        }

        SECTION("and 'Departure' type, then an exception is thrown. Spot must receive an 'Imminent Departure' note before accepting a 'Departure' note.")
        {
            // Box with boxId 99 leaves.
            // 'Departure' type is 3.
            BoardNote departureNote{99, SpotType::left};

            REQUIRE_THROWS(spot.changeNote(departureNote));
            REQUIRE(10 == spot.getBoardNote().getBoxId());
            REQUIRE(SpotType::to_arrive == spot.getBoardNote().getType());
        }
        
        SECTION("and 'Arrival' type, then an exception is thrown. Spot must receive an 'Imminent Arrival' note with the same boxId before accepting an 'Arrival' note.")
        {
            // Box with boxId 99 arrives.
            // 'Arrives' type is 4.
            BoardNote arrivalNote{99, SpotType::arrive};

            REQUIRE_THROWS(spot.changeNote(arrivalNote));
            REQUIRE(10 == spot.getBoardNote().getBoxId());
            REQUIRE(SpotType::to_arrive == spot.getBoardNote().getType());
        }
    }
}

TEST_CASE("Spot:: Spot originally has the type 'Arrived'")
{
    // Box with boxId 10 has arrived.
    // 'Arrival' type is 4.
    BoardNote imminentArrivalNote{10, SpotType::to_arrive};
    BoardNote arrivalNote{10, SpotType::arrive};
    Spot spot{Position{100, 200}};
    spot.changeNote(imminentArrivalNote);
    spot.changeNote(arrivalNote);

    SECTION("and receives a note with the same boxId")
    {
        SECTION("and 'Imminent Departure' type, then the Spot's type will change to type 1.")
        {
            // Box with boxId 10 is about to leave.
            // 'Imminent Departure' type is 1.
            BoardNote imminentDepartureNote{10, SpotType::to_leave};

            REQUIRE(true == spot.changeNote(imminentDepartureNote).second);
            REQUIRE(10 == spot.getBoardNote().getBoxId());
            REQUIRE(SpotType::to_leave == spot.getBoardNote().getType());
        }
        
        SECTION("and 'Imminent Arrival' type, then an exception is thrown because the box can not be about to arrive, it has already arrived.")
        {
            // Box with boxId 10 is about to arrive.
            // 'Imminent Arrival' type is 2.
            BoardNote imminentArrivalNote{10, SpotType::to_arrive};

            REQUIRE_THROWS(spot.changeNote(imminentArrivalNote));
            REQUIRE(10 == spot.getBoardNote().getBoxId());
            REQUIRE(SpotType::arrive == spot.getBoardNote().getType());
        }

        SECTION("and 'Departure' type, then an exception is thrown. A Spot's type must be 'Imminent Departure' to accept a 'Departure' note.")
        {
            // Box with boxId 10 leaves.
            // 'Departure' type is 3.
            BoardNote departureNote{10, SpotType::left};

            REQUIRE_THROWS(spot.changeNote(departureNote));
            REQUIRE(10 == spot.getBoardNote().getBoxId());
            REQUIRE(SpotType::arrive == spot.getBoardNote().getType());
        }
        
        SECTION("and 'Arrival' type, then an exception is thrown. Spot's type must be 'Imminent Arrival' to accept an 'Arrival' note.")
        {
            // Box with boxId 10 arrives.
            // 'Arrival' type is 4.
            BoardNote arrivalNote{10, SpotType::arrive};

            REQUIRE_THROWS(spot.changeNote(arrivalNote));
            REQUIRE(10 == spot.getBoardNote().getBoxId());
            REQUIRE(SpotType::arrive == spot.getBoardNote().getType());
        }
    }

    SECTION("and receives a note with a different box (different boxId)")
    {
        SECTION("and 'Imminent Departure' type, then an exception is thrown. Spot's type  must be 'Arrival' to accept an 'Imminent Departure' note.")
        {
            // Box with boxId 99 is about to leave.
            // 'Imminent Departure' type is 1.
            BoardNote imminentDepartureNote{99, SpotType::to_leave};

            REQUIRE_THROWS(spot.changeNote(imminentDepartureNote));
            REQUIRE(10 == spot.getBoardNote().getBoxId());
            REQUIRE(SpotType::arrive == spot.getBoardNote().getType());
        }

        SECTION("and 'Imminent Arrival' type, then false is returned. Two boxes can not be at the same spot.")
        {
            // Box with boxId 99 is about to arrive.
            // 'Imminent Arrival' type is 2.
            BoardNote imminentArrivalNote{99, SpotType::to_arrive};

            REQUIRE_FALSE(spot.changeNote(imminentArrivalNote).second);
            REQUIRE(10 == spot.getBoardNote().getBoxId());
            REQUIRE(SpotType::arrive == spot.getBoardNote().getType());
        }

        SECTION("and 'Departure' type, then an exception is thrown. Spot's type must be 'Imminent Departure' to accept a 'Departure' note.")
        {
            // Box with boxId 99 leaves.
            // 'Departure' type is 3.
            BoardNote departureNote{99, SpotType::left};

            REQUIRE_THROWS(spot.changeNote(departureNote));
            REQUIRE(10 == spot.getBoardNote().getBoxId());
            REQUIRE(SpotType::arrive == spot.getBoardNote().getType());
        }
        
        SECTION("and 'Arrival' type, then an exception is thrown. Spot's type  must be 'Imminent Arrival' to accept an 'Arrival' note.")
        {
            // boxId 99 arrives. "Arrives" type is 4.
            BoardNote arrivalNote{99, SpotType::arrive};

            REQUIRE_THROWS(spot.changeNote(arrivalNote));
            REQUIRE(10 == spot.getBoardNote().getBoxId());
            REQUIRE(SpotType::arrive == spot.getBoardNote().getType());
        }

    }
}

void funcChangeSpot(Spot& spot, int boxId)
{
    for (int ii=0; ii<10000; ++ii)
    {
        bool successful = false;

        try{
           successful =  spot.changeNote(BoardNote{boxId, SpotType::to_arrive}).second;
        }
        catch(...)
        {
        }
        if (successful)
        {
            try{
                spot.changeNote(BoardNote{boxId, SpotType::arrive});
            }
            catch(...)
            {
            }
            try{
                spot.changeNote(BoardNote{boxId, SpotType::to_leave});
            }
            catch(...)
            {
            }
            try{
                spot.changeNote(BoardNote{boxId, SpotType::left});
            }
            catch(...)
            {
            }
        }
    }
}

void funcReadSpot(Spot& spot)
{
    for (int ii=0; ii<10000; ++ii)
    {
        BoardNote note = spot.getBoardNote();
        if (note.getType() == SpotType::left)
        {
            REQUIRE( note.getBoxId() == -1 );
        }
        if (note.getBoxId() == -1)
        {
            REQUIRE( note.getType() == SpotType::left );
        }
    }
}

// Remove changeNotes() unique_lock to make this test fail 90% of the time.

// Removing the unique_lock may not only fail the test, but result in the following error: RunTests(22617,0x16d58f000) malloc: Region cookie corrupted for region 0x102e00000 (value is 7000001)[0x102e0407c] RunTests(22617,0x16d58f000) malloc: *** set a breakpoint in malloc_error_break to debug

TEST_CASE("two threads repeatedly try to change Spot's Note, but because of Spot's unique_lock on changeNote(), one thread always waits for the other thread to finish.")
{
    // If two threads are in the changeNote() method at the same time, one will be updating the _combinedString, while the other is returning what it believes to be the completed _combinedString.

    // When the Type is '4' corresponding to SpotType::left, then the boxId must be -1. So valid strings are B200,T2 or B100,T1. But a string of B100,T4 or B-1,T2 is invalid. If this happens, then two threads were inside the changeNote() method at the same time.

    std::cmatch match;
    std::regex reg("^(B)(-1|\\d+)(,T)(\\d)$");

    Spot spot{Position{8, 8}};
    SpotListener listener{};
    spot.registerListener(&listener);
    std::thread t1(funcChangeSpot, std::ref(spot), 100);
    std::thread t2(funcChangeSpot, std::ref(spot), 200);

    t1.join();
    t2.join();

    for (string str : listener.getCombinedStrings())
    { 
        std::regex_match(str.c_str(), match, reg);
        if(match.str(2) == "-1")
        {
            REQUIRE("4" == match.str(4));
        }
        if(match.str(4) == "4")
        {
            REQUIRE("-1" == match.str(2));
        }
    }

}

// Remove getBoardNote()'s shared_lock to make this test fail.

// It still may not fail, so in changeNote() add this_thread::sleep_for(10ms) for the case when _type is SpotType::left and a "to arrive" note is received. This will make it evident that getBoard() is running at the same time as changeNote() because changeNote() will change the _boxId, then wait 10ms before changing the _type. This gives enough time for the thread in getBoardNote() to run and return a noticeably invalid state (a _type of SpotType::3, with a boxId that is not -1).

TEST_CASE("One thread repeatedly calls getBoardNote(), the other repeatedly calls changeNote(). Because changeNote() has a unique_lock and getBoardNote() has a shared_lock, getBoardNote() will never return a BoardNote that is half way done.")
{
    // If one thread is in changeNote() and the other thread is in getBoardNote(), then at some point getBoardNote() will return a BoardNote of {100, SpotType::left}. This means the BoardNote was in the middle of being updated, when it was returned by getBoardNote(). If the BoardNote's type is Spot::left, then the complete state doesn't have a boxId, so boxId should be -1."

    Spot spot{Position{8, 8}};
    
    std::thread t1(funcReadSpot, std::ref(spot));
    std::thread t2(funcChangeSpot, std::ref(spot), 100);

    t2.join();
    t1.join();

}

