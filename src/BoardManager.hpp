#ifndef BOARDMANAGER_HPP
#define BOARDMANAGER_HPP

#include <SDL.h>
#include "Board.hpp"
#include "constants.hpp"
#include "Gem.hpp"

class BoardManager {

private:
    Board board;

    int start_x;
    int start_y;
    int end_x;
    int end_y;

public:
    BoardManager(int x, int y, int width, int height);

    void update();
    void draw(SDL_Renderer *renderer);

};

#endif // BOARDMANAGER_HPP