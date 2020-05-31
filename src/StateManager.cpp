#include "StateManager.hpp"

void StateManager::update() {
    this->currentState.update();
}

void StateManager::draw() {
    this->currentState.draw();
}
