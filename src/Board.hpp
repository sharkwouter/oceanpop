#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <SDL.h>
#include "Shell.hpp"

class Board {

private:
    std::vector<std::vector<Shell>> shells;

    bool isWithinBounds(SDL_Point point);
    std::vector<Shell> getMatches(std::vector<std::vector<Shell>> shells, std::vector<SDL_Point> * matchedGems);
    std::vector<std::vector<Shell>> getShellsCopy();

    int getCount(Shell shell);
public:
    Board(int width, int height);

    std::vector<Shell> match();

    void fillEmpty();

    bool hasEmpty();

    bool swap(SDL_Point p1, SDL_Point p2);

    int getWidth();
    int getHeight();

    std::vector<std::vector<Shell>> getShells(){return shells;};
    std::vector<std::vector<Shell>> getShellsAfterSwap(std::vector<std::vector<Shell>> shells, SDL_Point p1, SDL_Point p2);
};

#endif // BOARD_HPP
