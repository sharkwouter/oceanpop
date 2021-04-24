#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <SDL.h>
#include "BaseState.hpp"
#include "../BoardManager.hpp"

class GameState : public BaseState {
private:
    BoardManager board;
public:
    GameState();
    ~GameState();

    void update();
    void draw(SDL_Renderer *renderer);
};

#endif // GAMESTATE_HPP
