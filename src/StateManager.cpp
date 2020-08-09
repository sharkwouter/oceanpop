#include "StateManager.hpp"

StateManager::StateManager() {}

StateManager::~StateManager() {}

void StateManager::update() {
    this->currentState.update();
}

void StateManager::draw(SDL_Renderer *renderer) {
    this->currentState.draw(renderer);
}
