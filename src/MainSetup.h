#ifndef MAINSETUP__H
#define MAINSETUP__H

#include <vector>

#include "Box.h"
#include "Position.h"

class MainSetup
{
public:
    
    static std::vector<Box> getBoxes(int firstBoxId, int numOfGroups, int groupSize);
    static std::vector<std::pair<Position, Position>> getEndRectangles();
};

#endif
