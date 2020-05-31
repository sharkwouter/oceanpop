#include "StateManager.hpp"

StateManager::StateManager() {
}

void StateManager::update() {
    this->currentState.update();
}

void StateManager::draw() {
    currentState.draw();
}

StateManager::~StateManager() {

}
