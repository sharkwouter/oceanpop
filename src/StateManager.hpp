#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP

#include <SDL.h>
#include <vector>
#include "states/BaseState.hpp"
#include "Event.hpp"

class StateManager {
private:
    BaseState *currentState;
public:
    StateManager(BaseState *initial_state);
    ~StateManager();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer *renderer);
};

#endif // STATEMANAGER_HPP
