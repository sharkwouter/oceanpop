#include "GameState.hpp"

GameState::GameState(SDL_Renderer *renderer) :
    board(renderer, (SCREEN_WIDTH-SHELL_SIZE*BOARD_WIDTH)/2, (SCREEN_HEIGHT-SHELL_SIZE*(BOARD_HEIGHT+1))/2, BOARD_WIDTH, BOARD_HEIGHT),
    theme(renderer, 1, true)
{

}

GameState::~GameState() {

}

void GameState::update() {
    this->theme.update();
    this->board.update();
}

void GameState::handleEvents(std::vector<Event> events) {
    this->board.handleEvents(events);
}

void GameState::draw(SDL_Renderer *renderer) {
    this->theme.draw(renderer);
    this->board.draw(renderer);
}
