#ifndef BASESTATE_HPP
#define BASESTATE_HPP

#include "../Event.hpp"

class BaseState {
public:
    virtual ~BaseState() {}

    virtual void handleEvents(std::vector<Event> events) = 0;
    virtual void update() = 0;
    virtual void draw(SDL_Renderer *renderer) = 0;

    virtual bool isRunning() = 0;
};

#endif // BASESTATE_HPP
