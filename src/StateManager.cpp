#include "StateManager.hpp"

StateManager::StateManager() {
    currentState = GameState();
}

void StateManager::update() {
    currentState.update();
}

void StateManager::draw() {
    currentState.draw();
}

StateManager::~StateManager() {

}