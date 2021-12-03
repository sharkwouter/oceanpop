#include "StateManager.hpp"


#include "states/GameState.hpp"
#include "states/MenuState.hpp"
#include "states/State.hpp"

StateManager::StateManager(SDL_Renderer * renderer) : renderer(renderer), state(std::make_unique<MenuState>(renderer)) {

}

StateManager::~StateManager() {}

void StateManager::update() {
    if (!this->state->isDone()) {
        this->state->update();
    } else {
        switch (this->state->getNextState()) {
            case State::MENU:
                clearState();
                this->state = std::make_unique<MenuState>(renderer);
                break;
            case State::STANDARD:
                clearState();
                this->state = std::make_unique<GameState>(renderer);
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
    BaseState * old_state = this->state.release();
    delete old_state;
}
