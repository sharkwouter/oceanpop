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

    int position = 0;
    int cache_size = 10;
    int empty_line_height;
    int last_line_visible;

    std::vector<std::string> credits;

    State next_state = State::MENU;

    void loadCredits();
    void freeUnusedTexts();
    void loadTitles();
    void updateSizing();
public:
    CreditsState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options);
    ~CreditsState();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer * renderer);

    State getNextState();
};

#endif // CREDITSSTATE_HPP
