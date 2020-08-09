#include "GameState.hpp"

GameState::GameState() : board(Board(0, 0, 8, 7)) {}

GameState::~GameState() {}

void GameState::update() {
    this->board.update();
}

void GameState::draw(SDL_Renderer *renderer) {
    this->board.draw(renderer);
}
