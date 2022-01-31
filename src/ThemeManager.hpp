#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>

#include "OptionManager.hpp"

#include "Theme.hpp"

class ThemeManager {
private:
    Theme theme;
    SDL_Renderer * renderer;

    Mix_Music * music = NULL;
    Mix_Music * next_music = NULL;
    SDL_Texture * background = NULL;

    #ifdef __PSP__
        std::string music_file_type = "ogg";
    #else
        std::string music_file_type = "mp3";
    #endif

    bool paused = false;

    bool change_music_on_switch;
    int volume;
    int current_volume = 0;

    bool isRelaxedMode = false;

    void load(Theme theme);
    void loadBackground(Theme theme);
    void loadMusic(Theme theme);

    Theme getNextTheme();

    SDL_Texture * createBackgroundTexture(std::string filename);
public:
    ThemeManager(SDL_Renderer * renderer, Theme theme);
    ThemeManager(SDL_Renderer * renderer, OptionManager * options, Theme theme, bool isRelaxedMode=false);
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
