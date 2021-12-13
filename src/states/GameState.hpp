#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <vector>
#include <SDL.h>
#include "BaseState.hpp"
#include "../BoardManager.hpp"
#include "../ThemeManager.hpp"
#include "../PauseScreen.hpp"

class GameState : public BaseState {
private:
    SDL_Renderer * renderer;

    BoardManager * board;
    ThemeManager theme;
    FontManager fonts;

    PauseScreen pause_screen;
    PauseScreen win_screen;
    PauseScreen lose_screen;

    bool paused = false;
    bool completed = false;

    int level;
    int moves;
    int required_matches;
    int width;
    int height;
    SDL_Point position;

    SDL_Point calculatePosition(int width, int height);
    void loadLevel();
public:
    GameState(SDL_Renderer *renderer);
    ~GameState();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer *renderer);

    State getNextState();
};

#endif // GAMESTATE_HPP
