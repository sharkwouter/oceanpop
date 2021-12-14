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

    bool paused = false;

    void load(Theme theme);
    void loadBackground(Theme theme);
    void loadMusic(Theme theme);

    SDL_Texture * createBackgroundTexture(std::string filename);
public:
    ThemeManager(SDL_Renderer * renderer, Theme theme);
    ~ThemeManager();

    void update();
    void draw(SDL_Renderer * renderer);

    void next();
    void nextSong();
    void pause();
    void unpause();
};

#endif // THEMEMANAGER_H
