#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP

#include "states/BaseState.hpp"

class StateManager {
private:
    BaseState *currentState;
public:
    StateManager(BaseState *initial_state);
    ~StateManager();

    void update();
    void draw();
};

#endif // STATEMANAGER_HPP
