#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>

#include "Theme.hpp"

class ThemeManager {
private:
    Theme theme;

    Mix_Music * music;
    SDL_Texture * background;

    void load(SDL_Renderer * renderer, Theme theme);
    SDL_Texture * loadBackground(SDL_Renderer * renderer, std::string filename);
public:
    ThemeManager(SDL_Renderer * renderer, Theme theme);
    ~ThemeManager();

    void update();
    void draw(SDL_Renderer * renderer);
};

#endif // THEMEMANAGER_H
