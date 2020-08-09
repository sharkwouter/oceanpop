#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP

#include "states/BaseState.hpp"
#include <SDL.h>

class StateManager {
private:
    BaseState *currentState;
public:
    StateManager(BaseState *initial_state);
    ~StateManager();

    void update();
    void draw(SDL_Renderer *renderer);
};

#endif // STATEMANAGER_HPP
