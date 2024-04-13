#include "Board.h"
#include "BoardListener.h"
#include <thread>
#include <chrono>

using namespace std;

Board::Board(int width, int height, vector<Box> boxes)
:   _width{width},
    _height{height}
{
    for(int row=0; row<height; ++row)
    {
        vector<Spot> rowOfSpots{};
        vector<Drop> rowOfDrops1{};
        vector<Drop> rowOfDrops2{};
        for(int col=0; col<width; ++col)
        {
            rowOfSpots.push_back(Spot{Position{col, row}});
            rowOfDrops1.push_back(Drop{col, row});
            rowOfDrops2.push_back(Drop{col, row});
        }
        _spots.push_back(rowOfSpots);
        _dropMatrix1.push_back(rowOfDrops1);
        _dropMatrix2.push_back(rowOfDrops2);
    }
    _receivingMatrix = &_dropMatrix1;
    for(const Box& box : boxes)
    {  
        if(box.getId() != -1)
        {
            _boxes.insert({box.getId(), box});
        }
    }
}

int Board::getWidth() const
{
    return _width;
}

int Board::getHeight() const
{
    return _height;
}

// Note many threads can be inside addNote() at the same time, it is protected by a shared_lock.
// Although many threads can be inside addNote() at the same time, only one thread can change a particular spot at one time. Spot's changeNote() method is protected by a unique_lock.

// The shared_lock protecting addNote() will not allow changes to any Spot or any Drop while the _receivingMatrix is being toggled.

// A particular Spot and its corresponding Drop in _recevingMatrix should be changing in unison. Spot's changes are protected with locks inside Spot's methods. Drop doesn't have any protections. However, threads (through their contained PositionManager) contain a particular boxId. Once a thread has successfully changed a Spot with the "to arrive" type and a particular boxId, the Spot is essentially stamped with that particular boxId. Only that particular thread can change that Spot because only that particular thread has that boxId. At that point only that particular thread changes the corresponding Drop's attributes. Not until that particular thread changes the Spot's type to "left" and finishes the addNote() method can another thread change that particular Spot or its corresponding Drop.
bool Board::addNote(Position position, BoardNote newNote)
{
    shared_lock<shared_mutex> shLock(_mux);
    //TODO test that this exception is thrown.
    if(_boxes.find(newNote.getBoxId()) == _boxes.end())
    {
        string str = "Trying to add a BoardNote with a boxId of ";
        str.append(to_string(newNote.getBoxId()));
        str.append(" when there is no Box with that boxId.");
        throw(invalid_argument(str));
    }

    pair<int, bool> success = _spots[position.getY()][position.getX()].changeNote(newNote);

    if (!success.second)
    {
        //cout << "(" << _boxes[success.first].getLevel() << ", ";
        _boxes[success.first].upLevel();
        //cout << _boxes[success.first].getLevel() << ")  ";
        //cout << "[" << _boxes[newNote.getBoxId()].getLevel() << ", ";
        _boxes[newNote.getBoxId()].upLevel();
        //cout << _boxes[newNote.getBoxId()].getLevel() << "]  ";

        return false; 
    }
    else 
    {
        // Record changed boxId and type at position in _receivingMatrix.
        BoardNote currentBoardNote = _spots[position.getY()][position.getX()].getBoardNote();
        Drop& drop = (*_receivingMatrix)[position.getY()][position.getX()];
        drop._changed = true;
        drop._boxId = currentBoardNote.getBoxId();
        //std::this_thread::sleep_for(1ms);
        drop._type = currentBoardNote.getType();

        // Notify all BoardCallbacks. Should be none as BoardCallbacks are only used in testing.
        if (_boardCallbacksPerPos.find(position) != _boardCallbacksPerPos.end())
        {
            _boardCallbacksPerPos.at(position).callback(newNote, position);
        }
        return true;
    }
}

void Board::registerCallback(Position pos, BoardCallback& callBack)
{
    _boardCallbacksPerPos.insert({pos, callBack});
}

void Board::sendChanges()
{   
    // The sendChangesLock prevents two threads entering sendChanges() method at the same time.
    unique_lock<shared_mutex> sendChangesLock(_sendChangesMutex);
    vector<vector<Drop>>* changedBoard = nullptr;
    unordered_map<int, Box> copyOfBoxes{};
    {
        unique_lock<shared_mutex> lockUq(_mux);
        changedBoard = _receivingMatrix;
        _receivingMatrix = (_receivingMatrix == &_dropMatrix1) ? (&_dropMatrix2) : (&_dropMatrix1);
        
        // copy _boxes to send
        for(const auto& p : _boxes)
        {
            //cout << p.second.getLevel() << ", ";
            copyOfBoxes.insert({p.first, p.second});
        }
    }
    //cout << "copyOfBoxes.size()" << copyOfBoxes.size() << endl;

    // changedBoard holds the current matrix where changes are being made. 
  
    // TODO return setOfDropsPerPosition changing to per type should be done by the listener
    // Collect changes in setsOfDropsPerType  
    unordered_map<SpotType, unordered_set<Drop>> setsOfDropsPerType;

    for (int row=0; row<_height; ++row)
    {
        for (int col=0; col<_width; ++col)
        {
            Drop curDrop = (*changedBoard)[row][col];
            if (curDrop._changed)
            {
                setsOfDropsPerType[curDrop._type].insert(curDrop);
                (*changedBoard)[row][col]._boxId = -1;
                (*changedBoard)[row][col]._type = SpotType::left;
                (*changedBoard)[row][col]._changed = false;
            }
        }
    }

    for(BoardListener* listener : _listeners)
    {
        listener->receiveChanges(setsOfDropsPerType, copyOfBoxes);
    }
}

void Board::registerListener(BoardListener* listener)
{
    _listeners.insert(listener);
}

// TODO check that this shouldn't be locked. Maybe it should be a shared lock
BoardNote Board::getNoteAt(Position position) const
{
    shared_lock<shared_mutex> lock(_mux);
    return _spots[position.getY()][position.getX()].getBoardNote();
}

void Board::toggleReceivingMatrix()
{
    unique_lock<shared_mutex> lockUq(_mux);    
    _receivingMatrix = (_receivingMatrix == &_dropMatrix1) ? (&_dropMatrix2) : (&_dropMatrix1);
}
