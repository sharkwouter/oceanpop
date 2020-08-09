#ifndef BASESTATE_HPP
#define BASESTATE_HPP

class BaseState {
public:
    virtual ~BaseState() {}
    virtual void update() = 0;
    virtual void draw(SDL_Renderer *renderer) = 0;
};

#endif // BASESTATE_HPP
