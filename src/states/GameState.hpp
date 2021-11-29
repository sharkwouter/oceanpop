#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <vector>
#include <SDL.h>
#include "BaseState.hpp"
#include "../BoardManager.hpp"
#include "../ThemeManager.hpp"

class GameState : public BaseState {
private:
    BoardManager board;
    ThemeManager theme;

public:
    GameState(SDL_Renderer *renderer);
    ~GameState();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer *renderer);
};

#endif // GAMESTATE_HPP
