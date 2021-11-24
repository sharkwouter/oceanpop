#include "GameState.hpp"

GameState::GameState(SDL_Renderer *renderer) : board(renderer, (SCREEN_WIDTH-GEM_SIZE*BOARD_WIDTH)/2, 0, BOARD_WIDTH, BOARD_HEIGHT), textures() {
    textures.add_texture(background_image, renderer);
}

GameState::~GameState() {}

void GameState::update() {
    this->board.update();
}

void GameState::handleEvents(std::vector<Event> events) {
    this->board.handleEvents(events);
}

void GameState::draw(SDL_Renderer *renderer) {
    SDL_RenderCopy(renderer, textures.get(background_image), NULL, NULL);
    this->board.draw(renderer);
}
