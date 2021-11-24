#include "GameState.hpp"

GameState::GameState(SDL_Renderer *renderer) : board(renderer, (SCREEN_WIDTH-GEM_SIZE*BOARD_WIDTH)/2, (SCREEN_HEIGHT-GEM_SIZE*BOARD_HEIGHT)/2, BOARD_WIDTH, BOARD_HEIGHT) {

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
