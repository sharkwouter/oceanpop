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

    int level = 0;
    bool level_updated = true;

    int score = 0;
    bool score_updated = true;

    int required_score;

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
    SDL_Texture * text_level;


    void addScore(int matches);
    void increasLevel();
    void increaseMoves();
    void decreaseMoves();

    void moveCursor(int x, int y);
    void moveCursorMouse();

    void match();

    void drawCursor(SDL_Renderer * renderer);
    void drawBoard(SDL_Renderer * renderer);
    void drawScore(SDL_Renderer * renderer);
    void drawLevel(SDL_Renderer * renderer);
    void drawShells(SDL_Renderer * renderer);

    void init();
public:
    BoardManager(SDL_Renderer *renderer, FontManager *fonts, int x, int y, int width, int height);

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer *renderer);

    bool isCompleted() {return this->current_action == Action::COMPLETED;};
    void reset();
};

#endif // BOARDMANAGER_HPP
