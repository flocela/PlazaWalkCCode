#ifndef RECORDERLISTENER__H
#define RECORDERLISTENER__H

#include <unordered_map>
#include <unordered_set>
#include "Box.h"
#include "Drop.h"

class RecorderListener 
{
public:
    virtual void receiveAllDrops(std::unordered_map<SpotType, std::unordered_set<Drop>> setOfDropsPerType, std::unordered_map<int, Box> boxesPerBoxId) = 0;
};

#endif

