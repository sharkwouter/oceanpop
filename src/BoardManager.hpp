#ifndef BOARDMANAGER_HPP
#define BOARDMANAGER_HPP

#include <SDL.h>
#include "Board.hpp"
#include "constants.hpp"
#include "Gem.hpp"
#include "TextureManager.hpp"

class BoardManager {

private:
    Board board;

    int start_x;
    int start_y;
    int end_x;
    int end_y;

    std::string image_gems = "images/gems.png";

    TextureManager textures;

public:
    BoardManager(SDL_Renderer *renderer, int x, int y, int width, int height);

    void update();
    void draw(SDL_Renderer *renderer);

};

#endif // BOARDMANAGER_HPP