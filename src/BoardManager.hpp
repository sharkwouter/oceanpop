#ifndef BOARDMANAGER_HPP
#define BOARDMANAGER_HPP

#include <vector>
#include <SDL.h>
#include "Board.hpp"
#include "constants.hpp"
#include "Shell.hpp"
#include "TextureManager.hpp"
#include "FontManager.hpp"
#include "Event.hpp"
#include "Action.hpp"

class BoardManager {

private:
    Board board;

    int start_x;
    int start_y;
    int end_x;
    int end_y;

    SDL_Point selected;
    SDL_Point picked;

    int score;
    int required_score;
    int moves;

    std::string image_shells = "assets/images/shells.png";

    TextureManager textures;

    FontManager fonts;

    Action current_action;

    std::vector<std::vector<Shell>> preview;

    void addScore(int matches);

    void moveCursor(int x, int y);

    void drawCursor(SDL_Renderer * renderer);
    void drawBoard(SDL_Renderer * renderer);
    void drawScore(SDL_Renderer * renderer);
    void drawShells(SDL_Renderer * renderer);

    void init();
    void reset();
public:
    BoardManager(SDL_Renderer *renderer, int x, int y, int width, int height);

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer *renderer);

};

#endif // BOARDMANAGER_HPP
