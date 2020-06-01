#include "Gem.hpp"
#include "utils.hpp"

Gem::Gem(int x, int y, SDL_Texture *texture, SDL_Rect &src, int dst_x, int dst_y) {
    this->texture = texture;
    this->src = src;
    this->x = dst_x;
    this->y = dst_y;
}

void Gem::update() {}

void Gem::draw(SDL_Renderer *renderer) {
    SDL_Rect dst;
    dst.x = this->x;
    dst.y = this->y;
    dst.w = this->src.w;
    dst.h = this->src.h;

    SDL_RenderCopy(renderer, this->texture, &this->src, &dst);
}
