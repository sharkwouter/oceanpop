#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>

class ThemeManager {
private:
    int next_track;
    int repeat;

    Mix_Music * music;
    SDL_Texture * background;

    void loadMusic();
    void loadBackground(SDL_Renderer * renderer, int theme);
    SDL_Texture * loadTexture(SDL_Renderer * renderer, std::string path);
public:
    ThemeManager(SDL_Renderer * renderer, int theme, bool repeat);
    ~ThemeManager();

    void update();
    void draw(SDL_Renderer * renderer);
};

#endif // THEMEMANAGER_H
