#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include <string>
#include <SDL.h>

#include "OptionManager.hpp"

class TextureManager {
private:
    OptionManager * options;

    SDL_Texture* texture_shell;
public:
    TextureManager(SDL_Renderer * renderer, OptionManager * options);
    ~TextureManager();

    SDL_Texture * load(SDL_Renderer * renderer, const std::string &file);
    SDL_Texture * getShellTexture() { return texture_shell;};
};

#endif // TEXTUREMANAGER_HPP
