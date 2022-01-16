#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <SDL.h>
#include "Shell.hpp"

class Board {

private:
    std::vector<std::vector<ShellType>> shells;

    unsigned int seed;

    bool isRelaxedMode;

    bool isWithinBounds(SDL_Point point);
    std::vector<Match> getMatches(std::vector<std::vector<ShellType>> shells);
    std::vector<std::vector<ShellType>> getShellsCopy();

    int getCount(ShellType shell);

    unsigned int rand();
    void dropNewShell(int x);
    bool isFalling(int x);
    bool shellsMatch(std::vector<std::vector<ShellType>> shells, SDL_Point p1, SDL_Point p2, SDL_Point p3);
public:
    Board(int width, int height, int seed, bool isRelaxedMode=false);
    Board(std::vector<std::vector<ShellType>> shells, int seed, bool isRelaxedMode=false);
    ~Board();

    std::vector<Match> match();

    std::vector<Shell> dropShells();

    bool hasEmpty();

    bool swap(SDL_Point p1, SDL_Point p2);

    int getWidth();
    int getHeight();

    std::vector<std::vector<ShellType>> getShells(){return shells;};
    std::vector<std::vector<ShellType>> getShellsAfterSwap(std::vector<std::vector<ShellType>> shells, SDL_Point p1, SDL_Point p2);

    unsigned int getCurrentSeed(){return seed;};
};

#endif // BOARD_HPP
