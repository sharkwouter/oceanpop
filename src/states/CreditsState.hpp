#ifndef CREDITSSTATE_HPP
#define CREDITSSTATE_HPP

#include <string>
#include <vector>
#include <SDL.h>
#include "BaseState.hpp"
#include "../ThemeManager.hpp"
#include "../FontManager.hpp"
#include "../SoundManager.hpp"
#include "../OptionManager.hpp"
#include "../PauseScreen.hpp"

class CreditsState : public BaseState {
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

    State next_state = State::MENU;

    int getTextY(int number);
    std::vector<std::string> loadCredits();
public:
    CreditsState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options);
    ~CreditsState();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer * renderer);

    State getNextState();
};

#endif // CREDITSSTATE_HPP
