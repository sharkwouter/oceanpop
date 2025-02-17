#ifndef HOWTOMATCHSTATE_HPP
#define HOWTOMATCHSTATE_HPP

#include <string>
#include <vector>
#include <SDL.h>
#include "BaseState.hpp"
#include "../ThemeManager.hpp"
#include "../FontManager.hpp"
#include "../SoundManager.hpp"
#include "../OptionManager.hpp"
#include "../PauseScreen.hpp"
#include "../TextureManager.hpp"

class HowToMatchState : public BaseState {
private:
    SDL_Renderer * renderer;
    FontManager * fonts;
    SoundManager * sounds;
    OptionManager * options;

    ThemeManager theme;
    TextureManager textures;

    SDL_Texture * text_title = NULL;
    SDL_Texture * text_bottom = NULL;
    SDL_Texture * text_match_shells = NULL;
    SDL_Texture * text_conclusion = NULL;
    SDL_Texture * text_match = NULL;

    int text_start_y;

    State next_state = State::HOWTOSCORE;

    void loadTexts();
    void updateSizing();
public:
    HowToMatchState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options);
    ~HowToMatchState();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer * renderer);

    State getNextState();
};

#endif // HOWTOMATCHSTATE_HPP
