#include "GameState.hpp"

GameState::GameState(SDL_Renderer *renderer) : board(renderer, 0, 0, 8, 7) {

}

GameState::~GameState() {}

void GameState::update() {
    this->board.update();
}

void GameState::handleEvents(std::vector<Event> events) {
    this->board.handleEvents(events);
}

void GameState::draw(SDL_Renderer *renderer) {
    this->board.draw(renderer);
}
