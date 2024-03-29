#include "Box.h"

using namespace std;

Box::Box (int id, int width, int height)
:   _id{id},
    _width{width},
    _height{height}
{}

bool Box::operator== (const Box& o) const
{
    return  _id == o._id &&
            _width == o._width &&
            _height == o._height;
}

int Box::getId() const
{
    return _id;
}

int Box::getHeight() const
{
    return _height;
}

int Box::getWidth() const
{
    return _width;
}

void Box::upLevel()
{
    ++_level;
}
