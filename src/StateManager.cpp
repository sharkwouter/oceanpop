#include "StateManager.hpp"

StateManager::StateManager(BaseState *initial_state) : currentState(initial_state) {}

StateManager::~StateManager() {}

void StateManager::update() {
    this->currentState->update();
}

<<<<<<< HEAD
void StateManager::draw(SDL_Renderer *renderer) {
    this->currentState.draw(renderer);
=======
void StateManager::draw() {
    this->currentState->draw();
>>>>>>> origin/master
}
