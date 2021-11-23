#ifndef BOARD_HPP
#define BOARD_HPP

#include <array>
#include <SDL.h>
#include "constants.hpp"
#include "GemArray.hpp"
#include "Gem.hpp"

class Board {

private:
    Gem gems[BOARD_WIDTH][BOARD_HEIGHT];
public:
    Board();

    int match();

    void fillEmpty();

    bool hasEmpty();

    void swap(SDL_Point p1, SDL_Point p2);

    GemArray getGems(){return gems;};
};

#endif // BOARD_HPP
