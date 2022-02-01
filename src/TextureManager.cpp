#include <SDL_image.h>
#include "TextureManager.hpp"
#include "utils.hpp"

TextureManager::TextureManager(SDL_Renderer * renderer, OptionManager * options) : options(options) {
    texture_shell = load(renderer, "assets/images/shells" + std::to_string(this->options->getShellSize()) + ".png");
}

TextureManager::~TextureManager() {
    SDL_DestroyTexture(texture_shell);
}

SDL_Texture * TextureManager::load(SDL_Renderer * renderer, const std::string &file) {
    SDL_Surface *img = IMG_Load(getResourcePath(file).c_str());

    if (img == nullptr) {
        panic("couldn't load image: " + std::string(IMG_GetError()));
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, img);
    SDL_FreeSurface(img);

    if (texture == nullptr) {
        panic("couldn't create texture from surface: " + std::string(SDL_GetError()));
    }
    return std::move(texture);
}

