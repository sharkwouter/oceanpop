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
    BoardManager board;
    ThemeManager theme;
    FontManager fonts;

    PauseScreen pause_screen;
    PauseScreen win_screen;
    PauseScreen lose_screen;

    bool paused = false;
    bool completed = false;
public:
    GameState(SDL_Renderer *renderer);
    ~GameState();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer *renderer);

    State getNextState();
};

#endif // GAMESTATE_HPP
