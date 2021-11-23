#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <SDL.h>
#include "Gem.hpp"

class Board {

private:
    std::vector<std::vector<Gem>> gems;
public:
    Board(int width, int height);

    int match();

    void fillEmpty();

    bool hasEmpty();

    void swap(SDL_Point p1, SDL_Point p2);

    int getWidth();
    int getHeight();

    std::vector<std::vector<Gem>> getGems(){return gems;};
};

#endif // BOARD_HPP
