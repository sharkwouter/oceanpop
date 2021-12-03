#include "StateManager.hpp"


#include "states/GameState.hpp"
#include "states/MenuState.hpp"
#include "states/State.hpp"

StateManager::StateManager(SDL_Renderer * renderer) : renderer(renderer), state(new MenuState(renderer)) {

}

StateManager::~StateManager() {
    clearState();
}

void StateManager::update() {
    if (!this->state->isDone()) {
        this->state->update();
    } else {
        switch (this->state->getNextState()) {
            case State::MENU:
                clearState();
                this->state = new MenuState(renderer);
                break;
            case State::STANDARD:
                clearState();
                this->state = new GameState(renderer);
                break;
            case State::EXIT:
                this->done = true;
                break;
            default:
                break;
        }
    }
}

void StateManager::handleEvents(std::vector<Event> events) {
    this->state->handleEvents(events);
}

void StateManager::draw(SDL_Renderer *renderer) {
    this->state->draw(renderer);
}

// This function does not work at the moment
void StateManager::clearState() {
    delete this->state;
}
