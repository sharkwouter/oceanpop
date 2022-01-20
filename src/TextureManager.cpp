#include <SDL_image.h>
#include "TextureManager.hpp"
#include "utils.hpp"

TextureManager::TextureManager() {}

TextureManager::~TextureManager() {
    for (auto &pair : this->textures) {
        if (pair.second != nullptr) {
            SDL_DestroyTexture(pair.second);
            pair.second = nullptr;
        }
    }
    this->textures.clear();
}

void TextureManager::add_texture(const std::string &file, SDL_Renderer *renderer) {
    SDL_Surface *img = IMG_Load(getResourcePath(file).c_str());

    if (img == nullptr) {
        panic("couldn't load image: " + std::string(IMG_GetError()));
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, img);
    SDL_FreeSurface(img);

    if (texture == nullptr) {
        panic("couldn't create texture from surface: " + std::string(SDL_GetError()));
    }

    this->textures[file] = texture;
}

SDL_Texture *TextureManager::get(const std::string &file) {
    return this->textures[file];
}
