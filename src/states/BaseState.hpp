#ifndef BASESTATE_HPP
#define BASESTATE_HPP

class BaseState {
public:
    virtual void update() = 0;
    virtual void draw() = 0;
};

#endif // BASESTATE_HPP