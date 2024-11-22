#ifndef BOARDMANAGER_HPP
#define BOARDMANAGER_HPP

#include <vector>
#include <SDL.h>
#include <SDL_mixer.h>
#include "Board.hpp"
#include "constants.hpp"
#include "Shell.hpp"
#include "TextureManager.hpp"
#include "FontManager.hpp"
#include "SoundManager.hpp"
#include "OptionManager.hpp"
#include "Event.hpp"
#include "Action.hpp"

class BoardManager {

private:
    FontManager * fonts;
    SoundManager * sounds;
    OptionManager * options;

    Board * board = NULL;

    TextureManager textures;

    SDL_Rect rect_board;
    SDL_Rect rect_scoreboard;
    SDL_Point selected;
    SDL_Point picked;

    int level = 0;
    bool level_updated = true;

    int seed;

    int matches = 0;
    bool matches_updated = true;

    int required_matches;
    bool isRelaxedMode = false;

    int moves;
    int starting_moves;
    bool moves_updated = true;

    std::vector<std::vector<ShellType>> starting_shells;

    bool mouse_active = false;

    bool bubbles_matched = false;

    Action current_action;

    std::vector<std::vector<ShellType>> preview;

    std::vector<Shell> shells_to_drop;
    bool done_falling = true;

    std::vector<Match> matches_made;

    SDL_Texture * text_moves = NULL;
    SDL_Texture * text_matches = NULL;
    SDL_Texture * text_level = NULL;
    SDL_Texture * text_introduction = NULL;

    SDL_Texture * text_plus_one = NULL;
    SDL_Texture * text_plus_two = NULL;
    SDL_Texture * text_minus_three = NULL;
    SDL_Texture * text_minus_six = NULL;
    SDL_Texture * text_plus_three = NULL;
    SDL_Texture * text_plus_six = NULL;

    SDL_Rect rect_matches;
    SDL_Rect rect_moves;

    int animation = 0;
    unsigned int animation_start;

    int previous_shell_size;

    void addMatches(int matches);
    void increaseLevel();
    void increaseMoves();
    void decreaseMoves();

    void moveCursor(int x, int y);
    void moveCursorMouse();

    void match();

    bool isFalling(SDL_Point point);
    bool isDoubleMatch(Match match);

    void setSizing();

    void drawCursor(SDL_Renderer * renderer);
    void drawBoard(SDL_Renderer * renderer);
    void drawScoringBox(SDL_Renderer * renderer);
    void drawInfo(SDL_Renderer * renderer);
    void drawShells(SDL_Renderer * renderer);
    void drawFallingShells(SDL_Renderer * renderer);
    void drawMatches(SDL_Renderer * renderer);
    void drawIntroduction(SDL_Renderer * renderer);
    void storeLevel(Board * board, int moves, int required_matches, int level);
public:
    BoardManager(SDL_Renderer *renderer, FontManager *fonts, SoundManager * sounds, OptionManager * options, int width, int height, int moves, int required_matches=0, int level=0, int seed=0);
    BoardManager(SDL_Renderer *renderer, FontManager *fonts, SoundManager * sounds, OptionManager * options, std::vector<std::vector<ShellType>> shells, int moves, int required_matches=0, int level=0, int seed=0);
    ~BoardManager();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer *renderer);

    bool isCompleted() {return (this->current_action == Action::COMPLETED);};
    int hasMovesLeft() {return (moves > 0);};
    void reset();
    void loadLevel(int width, int height, int moves, int required_matches=0, int level=0, int seed=0);
    void loadLevel(std::vector<std::vector<ShellType>> shells, int moves, int required_matches=0, int level=0, int seed=0);

    int getMatches() {return matches;};
    void setMatches(int matches) { this->matches = matches; this->matches_updated = true;};
    int getRemainingMoves() {return moves;};
    unsigned int getCurrentSeed() {return board->getCurrentSeed();};
    std::vector<std::vector<ShellType>> getCurrentShells() {return board->getShells();};
};

#endif // BOARDMANAGER_HPP
