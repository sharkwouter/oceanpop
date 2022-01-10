#ifndef HIGHSCORESTATE_HPP
#define HIGHSCORESTATE_HPP

#include <string>
#include <vector>
#include <SDL.h>
#include "BaseState.hpp"
#include "../ThemeManager.hpp"
#include "../FontManager.hpp"
#include "../SoundManager.hpp"
#include "../OptionManager.hpp"
#include "../PauseScreen.hpp"

class HighscoreState : public BaseState {
private:
    SDL_Renderer * renderer;
    FontManager * fonts;
    SoundManager * sounds;
    OptionManager * options;

    ThemeManager theme;

    SDL_Texture * text_title;
    SDL_Texture * text_bottom;
    std::vector<SDL_Texture*> texts;

    int text_offset = 2;
    int text_start_y;

    int getTextY(int number);
public:
    HighscoreState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options);
    ~HighscoreState();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer * renderer);

    State getNextState();
};

#endif // HIGHSCORESTATE_HPP
