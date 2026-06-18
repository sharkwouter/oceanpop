#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <string>

#include "OptionManager.hpp"

#include "Theme.hpp"

class ThemeManager {
private:
    SDL_Renderer * renderer;
    OptionManager * options;

    Theme theme;
    Theme music_theme;

    MIX_Mixer * mixer = nullptr;
    MIX_Audio * music = nullptr;
    MIX_Track * music_track = nullptr;
    SDL_Texture * background = nullptr;

    bool paused = false;

    bool change_music_on_switch;
    float volume;
    float current_volume = 0;

    void load(Theme theme);
    void loadBackground(Theme theme);
    void loadMusic(Theme theme);

    Theme getNextTheme();
    Theme getNextMusicTheme();

    SDL_Texture * createBackgroundTexture(std::string filename);
public:
    ThemeManager(SDL_Renderer * renderer, MIX_Mixer * mixer, OptionManager * options, Theme theme);
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
