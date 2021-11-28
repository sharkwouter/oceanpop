#include "GameState.hpp"

GameState::GameState(SDL_Renderer *renderer) : board(renderer, (SCREEN_WIDTH-GEM_SIZE*BOARD_WIDTH)/2, (SCREEN_HEIGHT-GEM_SIZE*(BOARD_HEIGHT+1))/2, BOARD_WIDTH, BOARD_HEIGHT), textures() {
    textures.add_texture(background_image, renderer);

    music = Mix_LoadMUS("assets/music/song1.mp3");
    Mix_VolumeMusic(MIX_MAX_VOLUME/2);
    Mix_PlayMusic(music, SDL_TRUE);
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
