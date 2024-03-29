#ifndef PRINTER_ONECOLOR__H
#define PRINTER_ONECOLOR__H

#include "RecorderListener.h"
#include "Recorder.h"
#include "SDL.h"

class Printer_OneColor : public RecorderListener {
public:
    Printer_OneColor(SDL_Renderer* renderer);
    Printer_OneColor() = delete;
    Printer_OneColor(const Printer_OneColor& o) = delete;
    Printer_OneColor(Printer_OneColor&& o) noexcept = delete;
    Printer_OneColor& operator=(const Printer_OneColor& printer) = delete;
    Printer_OneColor& operator=(Printer_OneColor&& o) noexcept = delete;
    ~Printer_OneColor() noexcept = default;

    void addEndPoint(Position topLeft, Position bottomRight);

    void receiveAllDrops(std::unordered_map<SpotType, std::unordered_set<Drop>> setOfDropsPerType, std::unordered_map<int, Box> boxesPerBoxId) override;

private:
    SDL_Renderer* _renderer;
    void print(std::unordered_map<SpotType, std::unordered_set<Drop>> dropsPerType, std::unordered_map<int, Box> boxesPerBoxId);
    std::vector<std::pair<Position, Position>> _endPoints{};
};

#endif
