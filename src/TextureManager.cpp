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
    SDL_Texture * texture = IMG_LoadTexture(renderer, getResourcePath(file).c_str());

    if (texture == nullptr) {
        panic("couldn't load image: " + std::string(IMG_GetError()));
    }

    return std::move(texture);
}

