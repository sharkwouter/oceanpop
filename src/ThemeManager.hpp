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
    Mix_Music * next_music = NULL;
    SDL_Texture * background = NULL;

    bool paused = false;

    bool change_music_on_switch = true;
    int volume;
    int current_volume = 0;

    void load(Theme theme);
    void loadBackground(Theme theme);
    void loadMusic(Theme theme);

    Theme getNextTheme();

    SDL_Texture * createBackgroundTexture(std::string filename);
public:
    ThemeManager(SDL_Renderer * renderer, Theme theme);
    ~ThemeManager();

    void update();
    void draw(SDL_Renderer * renderer);

    void next();
    void nextSong();

    void switchTheme(int theme);

    void pause();
    void unpause();
};

#endif // THEMEMANAGER_H
