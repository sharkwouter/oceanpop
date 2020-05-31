#include "StateManager.hpp"

StateManager::StateManager() {
}

void StateManager::update() {
    currentState.update();
}

void StateManager::draw() {
    currentState.draw();
}

StateManager::~StateManager() {

}
