#ifndef PAUSESCREEN_HPP
#define PAUSESCREEN_HPP

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

#include "FontManager.hpp"
#include "OptionManager.hpp"
#include "Event.hpp"

class PauseScreen {

private:
    FontManager * fonts;
    OptionManager * options;

    SDL_Texture * title = NULL;
    SDL_Texture * subtitle = NULL;

    std::string title_string;
    std::string subtitle_string;
public:
    PauseScreen(SDL_Renderer * renderer, FontManager * fonts, OptionManager * options, std::string title, std::string subtitle);
    ~PauseScreen();

    void updateSizing();
    void draw(SDL_Renderer *renderer);
};

#endif // PAUSESCREEN_HPP