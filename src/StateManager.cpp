#include "StateManager.hpp"

StateManager::StateManager(BaseState *initial_state) : currentState(initial_state) {}

StateManager::~StateManager() {}

void StateManager::update() {
    this->currentState->update();
}

void StateManager::handleEvents(std::vector<Event> events) {
    this->currentState->handleEvents(events);
}

void StateManager::draw(SDL_Renderer *renderer) {
    this->currentState->draw(renderer);
}
