#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP

#include <SDL.h>
#include <vector>
#include <memory>
#include "states/BaseState.hpp"
#include "Event.hpp"

class StateManager {
private:
    std::unique_ptr<BaseState> state;

    SDL_Renderer * renderer;

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
