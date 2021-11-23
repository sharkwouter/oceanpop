#ifndef BOARDMANAGER_HPP
#define BOARDMANAGER_HPP

#include <vector>
#include <SDL.h>
#include "Board.hpp"
#include "constants.hpp"
#include "Gem.hpp"
#include "TextureManager.hpp"
#include "Event.hpp"
#include "Action.hpp"

class BoardManager {

private:
    Board board;

    int start_x;
    int start_y;
    int end_x;
    int end_y;

    int selected_x;
    int selected_y;

    SDL_Point picked;

    std::string image_gems = "images/gems.png";

    TextureManager textures;

    Action current_action;

public:
    BoardManager(SDL_Renderer *renderer, int x, int y, int width, int height);

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer *renderer);

};

#endif // BOARDMANAGER_HPP
