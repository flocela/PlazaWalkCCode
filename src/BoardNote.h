#ifndef BOARDNOTE__H
#define BOARDNOTE__H

#include "Position.h"
#include "SpotType.h"

class BoardNote
{

public:
    BoardNote(int boxId, SpotType type);
    BoardNote() = delete;
    BoardNote(const BoardNote& o) = default;
    BoardNote(BoardNote&& o) noexcept = default;
    BoardNote& operator=(const BoardNote& o) = default;
    BoardNote& operator=(BoardNote&& o) noexcept = default;
    ~BoardNote() = default;

    SpotType getType() const;
    int getBoxId() const;

    bool operator== (const BoardNote& o) const;

private:
    SpotType _type;
    int _boxId;

};

// TODO find out how to call a hash function and how to test it.
namespace std
{
    template<>
    struct hash<BoardNote>
    {
        size_t operator()(const BoardNote& b) const
        {   return  (hash<int>()(static_cast<int>(b.getType())) ^ (hash<int>()(b.getBoxId()) << 1));
        }
    };
}

#endif
