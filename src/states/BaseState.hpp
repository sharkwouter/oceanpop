#ifndef BASESTATE_HPP
#define BASESTATE_HPP

#include <SDL.h>

#include "../Event.hpp"
#include "State.hpp"

class BaseState {
public:
    virtual ~BaseState() {};

    virtual void handleEvents(std::vector<Event> events) = 0;
    virtual void update() = 0;
    virtual void draw(SDL_Renderer *renderer) = 0;

    virtual bool isDone() {return done;};
    virtual State getNextState() {return State::EXIT;};
protected:
    bool done = false;
};

#endif // BASESTATE_HPP
