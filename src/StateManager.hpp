#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP

#include <SDL.h>
#include <vector>
#include "states/BaseState.hpp"
#include "Event.hpp"

class StateManager {
private:
    SDL_Renderer * renderer;

    BaseState * state;

    bool done = false;
    void clearState();
public:
    StateManager(SDL_Renderer * renderer);
    ~StateManager();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer *renderer);

    bool isDone() {return done;};
};

#endif // STATEMANAGER_HPP
