#ifndef PAUSESCREEN_HPP
#define PAUSESCREEN_HPP

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

#include "FontManager.hpp"
#include "Event.hpp"

class PauseScreen {

private:
    FontManager fonts;

    SDL_Texture * title = NULL;
    SDL_Texture * subtitle = NULL;
public:
    PauseScreen(SDL_Renderer * renderer, std::string title, std::string subtitle);
    ~PauseScreen();

    void draw(SDL_Renderer *renderer);
};

#endif // PAUSESCREEN_HPP