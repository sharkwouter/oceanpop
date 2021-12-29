#include "StateManager.hpp"


#include "states/GameState.hpp"
#include "states/GameStateChallenge.hpp"
#include "states/GameStateRelaxed.hpp"
#include "states/GameOverState.hpp"
#include "states/MenuState.hpp"
#include "states/NotImplementedState.hpp"
#include "states/WonState.hpp"
#include "states/State.hpp"

StateManager::StateManager(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds) : renderer(renderer), fonts(fonts), sounds(sounds), state(new MenuState(renderer, fonts, sounds)) {

}

StateManager::~StateManager() {
    clearState();
}

void StateManager::update() {
    if (!this->state->isDone()) {
        this->state->update();
    } else {
        switchState();
    }
}

void StateManager::handleEvents(std::vector<Event> events) {
    this->state->handleEvents(events);
}

void StateManager::draw(SDL_Renderer *renderer) {
    this->state->draw(renderer);
}

void StateManager::switchState() {
    State next_state = this->state->getNextState();
    if (next_state == State::EXIT) {
        this->done = true;
    } else {
        clearState();
        switch (next_state) {
            case State::MENU:
                this->state = new MenuState(renderer, fonts, sounds);
                break;
            case State::STANDARD:
                this->state = new GameState(renderer, fonts, sounds);
                break;
            case State::CHALLENGE:
                this->state = new GameStateChallenge(renderer, fonts, sounds);
                break;
            case State::RELAXED:
                this->state = new GameStateRelaxed(renderer, fonts, sounds);
                break;
            case State::GAMEOVER:
                this->state = new GameOverState(renderer, fonts, sounds);
                break;
            case State::WON:
                this->state = new WonState(renderer, fonts, sounds);
                break;
            default:
                this->state = new NotImplementedState(renderer, fonts, sounds);
                break;
        }
    }
}

void StateManager::clearState() {
    delete this->state;
}
