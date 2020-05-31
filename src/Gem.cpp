#include "Gem.hpp"
#include "utils.hpp"

Gem::Gem(int x, int y, GemType type, SDL_Renderer *renderer) {
    SDL_Surface *img = nullptr;
    switch (type) {
    case GemType::RED:
        img = SDL_LoadBMP("../assets/images/gems.png");
        break;
    case GemType::GREEN:
        img = SDL_LoadBMP("../assets/images/gems.png");
        break;
    case GemType::BLUE:
        img = SDL_LoadBMP("../assets/images/gems.png");
        break;
    }

    if (img == nullptr) {
        panic("couldn't load bmp file: " + std::string(SDL_GetError()));
    }

    this->texture = SDL_CreateTextureFromSurface(renderer, img);
    SDL_FreeSurface(img);

    if (this->texture == nullptr) {
        panic("couldn't create texture from surface: " + std::string(SDL_GetError()));
    }

    SDL_QueryTexture(this->texture, nullptr, nullptr, &this->rect.w, &this->rect.h);

    this->rect.x = x;
    this->rect.y = y;
}

void Gem::draw(SDL_Renderer *renderer) {
    SDL_RenderCopy(renderer, this->texture, nullptr, &this->rect);
}

void Gem::set_position(int x, int y) {
    this->rect.x = x;
    this->rect.y = y;
}

Gem::~Gem() {
    SDL_DestroyTexture(this->texture);
}
