#include "StateManager.hpp"

#include <typeinfo>

StateManager::StateManager(BaseState *initial_state) : currentState(initial_state) {
    this->running = true;
}

StateManager::~StateManager() {}

void StateManager::update() {
    this->currentState->update();

    if (!currentState->isRunning()) {
        if (auto* derived = dynamic_cast<GameState*>(this->currentState)) {
            this->running = false;
        }
    }
}

void StateManager::handleEvents(std::vector<Event> events) {
    this->currentState->handleEvents(events);
}

void StateManager::draw(SDL_Renderer *renderer) {
    this->currentState->draw(renderer);
}
