#ifndef BOARDMANAGER_HPP
#define BOARDMANAGER_HPP

#include <vector>
#include <SDL.h>
#include <SDL_mixer.h>
#include "Board.hpp"
#include "constants.hpp"
#include "Shell.hpp"
#include "TextureManager.hpp"
#include "SoundManager.hpp"
#include "FontManager.hpp"
#include "Event.hpp"
#include "Action.hpp"

class BoardManager {

private:
    Board board;

    SDL_Point start;
    SDL_Point end;
    SDL_Point selected;
    SDL_Point picked;

    int score;
    int required_score;
    bool score_updated = true;

    int moves;
    bool moves_updated = true;

    bool mouse_active = false;

    std::string image_shells = "assets/images/shells.png";

    TextureManager textures;
    SoundManager sounds;
    FontManager * fonts;

    Action current_action;

    std::vector<std::vector<Shell>> preview;

    SDL_Texture * text_moves;
    SDL_Texture * text_score;

    void addScore(int matches);
    void increaseMoves();
    void decreaseMoves();

    void moveCursor(int x, int y);
    void moveCursorMouse();

    void match();

    void drawCursor(SDL_Renderer * renderer);
    void drawBoard(SDL_Renderer * renderer);
    void drawScore(SDL_Renderer * renderer);
    void drawShells(SDL_Renderer * renderer);

    void init();
    void reset();
public:
    BoardManager(SDL_Renderer *renderer, FontManager *fonts, int x, int y, int width, int height);

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer *renderer);

};

#endif // BOARDMANAGER_HPP
