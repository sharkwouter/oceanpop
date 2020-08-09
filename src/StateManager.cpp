#include "StateManager.hpp"

StateManager::StateManager(BaseState *initial_state) : currentState(initial_state) {}

StateManager::~StateManager() {}

void StateManager::update() {
    this->currentState->update();
}

void StateManager::draw() {
    this->currentState->draw();
}
