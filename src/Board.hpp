#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <SDL.h>
#include "Gem.hpp"

class Board {

private:

public:
    Board(int width, int height);

    std::vector<std::vector<Gem>> gems;

    int getWidth();
    int getHeight();
};

#endif // BOARD_HPP
