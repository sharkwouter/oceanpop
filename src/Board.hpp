#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <SDL.h>
#include "Gem.hpp"

class Board {

private:
    std::vector<std::vector<Gem>> gems;

    bool isWithinBounds(SDL_Point point);
    int getMatches(std::vector<std::vector<Gem>> gems, std::vector<SDL_Point> * matchedGems);
    std::vector<std::vector<Gem>> getGemsCopy();
public:
    Board(int width, int height);

    int match();

    void fillEmpty();

    bool hasEmpty();

    bool swap(SDL_Point p1, SDL_Point p2);

    int getWidth();
    int getHeight();

    std::vector<std::vector<Gem>> getGems(){return gems;};
    std::vector<std::vector<Gem>> getGemsAfterSwap(std::vector<std::vector<Gem>> gems, SDL_Point p1, SDL_Point p2);
};

#endif // BOARD_HPP
