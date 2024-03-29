#include "Spot.h"
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

Spot::Spot(Position pos):_position{pos}
{}

Spot::Spot(const Spot& o): _position{o._position}
{
    _boxId = o._boxId;
    _type = o._type;
}

Spot::Spot(Spot&& o)noexcept:_position{o._position}
{
    _boxId = o._boxId;
    _type = o._type;
}
    

Position Spot::getPosition() const
{
    return _position;
}

pair<int, bool> Spot::changeNote(BoardNote note)
{
    unique_lock<shared_mutex> lock(_mm);

    int noteBoxId = note.getBoxId();
    SpotType noteType  = note.getType();
    
    if (_type == SpotType::left)
    { 
        if (noteType != SpotType::to_arrive)
        {   
            //cout << errorString(note) << endl;
            throw invalid_argument(errorString(note));
        }
        else
        {
            _boxId = noteBoxId;
            //this_thread::sleep_for(10ms);
            _type = noteType;
        }
    }
    else if (_type == SpotType::to_leave)
    {  
        if (noteType == SpotType::to_arrive && noteBoxId != _boxId)
        {
            return {_boxId, false};
        }
        else
        {
            if (_boxId == noteBoxId && noteType == SpotType::left)
            {
                _type = SpotType::left;
                _boxId = -1;
            }
            else
            {
                //cout << errorString(note) << endl;
                throw invalid_argument(errorString(note));
            }
        } 
    }
    else if (_type == SpotType::to_arrive)
    {  
        if (noteType == SpotType::to_arrive && noteBoxId != _boxId)
        {  
            return {_boxId, false};
        }
        else if (_boxId == noteBoxId && noteType == SpotType::arrive)
        {   
            _boxId = noteBoxId;
            _type = noteType;
        }
        else 
        {
            //cout << errorString(note) << endl;
            throw invalid_argument(errorString(note));
        } 
    }
    else if (_type == SpotType::arrive)
    {   
        if (noteType == SpotType::to_arrive && noteBoxId != _boxId)
        {
            return {_boxId, false};
        }
        else if (_boxId == noteBoxId && noteType == SpotType::to_leave)
        {
            _boxId = noteBoxId;
            _type = noteType;
        }
        else
        {
            //cout << errorString(note) << endl;
            throw invalid_argument(errorString(note));
        } 
    }
    
    notifyListeners();
    return {-1,true};
}

BoardNote Spot::getBoardNote() const
{
    shared_lock<shared_mutex> lock(_mm);
    return BoardNote{_boxId, _type};
}

void Spot::updateCombinedString()
{
        _combined = "B";
        _combined.append(to_string(_boxId));
        _combined.append(",T");
        _combined.append(to_string((int)(_type)));
}

void Spot::registerListener(SpotListener* listener)
{
    _listeners.push_back(listener);
}

void Spot::notifyListeners()
{
    for(SpotListener* listener: _listeners)
    {
        updateCombinedString();
        listener->receiveCombinedString(_combined);
    }
}

string Spot::errorString(BoardNote boardNote)
{ return "At {" + to_string(_position.getX()) + ", " + to_string(_position.getY()) + "} "  + " can not accept the received BoardNote with boxId of " + to_string(boardNote.getBoxId()) + " and type of "  + to_string(static_cast<int>(boardNote.getType())) + ". Current boxId and type are " + to_string(_boxId) + " and " + to_string(static_cast<int>(_type)) + ".";
}

