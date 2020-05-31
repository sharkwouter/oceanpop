#include "StateManager.hpp"

StateManager::StateManager() {
}

void StateManager::update() {
    this->currentState.update();
}

void StateManager::draw() {
    this->currentState.draw();
}

StateManager::~StateManager() {

}
