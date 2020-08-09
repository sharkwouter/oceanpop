#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP

#include "states/GameState.hpp"

class StateManager {
private:
    GameState currentState;
public:
    StateManager();
    ~StateManager();

    void update();
    void draw(SDL_Renderer *renderer);
};

#endif // STATEMANAGER_HPP
