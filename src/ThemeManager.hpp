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
    SDL_Renderer * renderer;

    Mix_Music * music = NULL;
    SDL_Texture * background;

    void load();
    SDL_Texture * loadBackground(std::string filename);
public:
    ThemeManager(SDL_Renderer * renderer, Theme theme);
    ~ThemeManager();

    void update();
    void draw(SDL_Renderer * renderer);

    void next();
};

#endif // THEMEMANAGER_H
