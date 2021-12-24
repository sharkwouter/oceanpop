#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <SDL.h>
#include "Shell.hpp"

class Board {

private:
    std::vector<std::vector<ShellType>> shells;

    bool isWithinBounds(SDL_Point point);
    std::vector<ShellType> getMatches(std::vector<std::vector<ShellType>> shells, std::vector<SDL_Point> * matchedGems);
    std::vector<std::vector<ShellType>> getShellsCopy();

    int getCount(ShellType shell);

    void dropNewShell(int x);
    bool isFalling(int x);
    bool shellsMatch(std::vector<std::vector<ShellType>> shells, SDL_Point p1, SDL_Point p2, SDL_Point p3);
public:
    Board(int width, int height, int seed);
    ~Board();

    std::vector<ShellType> match();

    std::vector<Shell> dropShells();

    bool hasEmpty();

    bool swap(SDL_Point p1, SDL_Point p2);

    int getWidth();
    int getHeight();

    std::vector<std::vector<ShellType>> getShells(){return shells;};
    std::vector<std::vector<ShellType>> getShellsAfterSwap(std::vector<std::vector<ShellType>> shells, SDL_Point p1, SDL_Point p2);
};

#endif // BOARD_HPP
