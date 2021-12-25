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
    FontManager * fonts;
    SoundManager * sounds;

    Board * board = NULL;

    TextureManager textures;

    SDL_Rect rect_board;
    SDL_Point selected;
    SDL_Point picked;

    int level = 0;
    bool level_updated = true;

    int seed;

    int matches = 0;
    bool matches_updated = true;

    int required_matches;

    int starting_moves;
    int moves;
    bool moves_updated = true;

    bool mouse_active = false;

    bool bubbles_matched = false;

    std::string image_shells = "assets/images/shells.png";

    Action current_action;

    std::vector<std::vector<ShellType>> preview;

    std::vector<Shell> shells_to_drop;
    bool done_falling = true;

    SDL_Texture * text_moves = NULL;
    SDL_Texture * text_matches = NULL;
    SDL_Texture * text_level = NULL;

    float animation = 0;

    void addMatches(int matches);
    void increasLevel();
    void increaseMoves();
    void decreaseMoves();

    void moveCursor(int x, int y);
    void moveCursorMouse();

    void match();

    bool isFalling(SDL_Point point);

    void drawCursor(SDL_Renderer * renderer);
    void drawBoard(SDL_Renderer * renderer);
    void drawInfo(SDL_Renderer * renderer);
    void drawShells(SDL_Renderer * renderer);
    void drawFallingShells(SDL_Renderer * renderer);
public:
    BoardManager(SDL_Renderer *renderer, FontManager *fonts, SoundManager * sounds, int x, int y, int width, int height, int moves, int required_matches=0, int level=0, int seed=0);
    ~BoardManager();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer *renderer);

    bool isCompleted() {return this->current_action == Action::COMPLETED;};
    int hasMovesLeft() {return (required_matches == 0 || moves > 0);};
    void reset();
    void loadLevel(int x, int y, int width, int height, int moves, int required_matches=0, int level=0, int seed=0);
};

#endif // BOARDMANAGER_HPP
