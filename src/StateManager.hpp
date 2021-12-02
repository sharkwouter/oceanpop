#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP

#include <SDL.h>
#include <vector>
#include "states/BaseState.hpp"
#include "states/GameState.hpp"
#include "Event.hpp"

class StateManager {
private:
    BaseState *currentState;

    bool running;
public:
    StateManager(BaseState *initial_state);
    ~StateManager();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer *renderer);

    bool isRunning() {return running;};
};

#endif // STATEMANAGER_HPP
