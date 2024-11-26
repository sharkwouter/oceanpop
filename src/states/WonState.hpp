#ifndef WONSTATE_HPP
#define WONSTATE_HPP

#include <SDL.h>

#include <vector>

#include "BaseState.hpp"
#include "../BoardManager.hpp"
#include "../ThemeManager.hpp"
#include "../FontManager.hpp"
#include "../SoundManager.hpp"
#include "../OptionManager.hpp"
#include "../PauseScreen.hpp"

class WonState : public BaseState {
private:
    SDL_Renderer * renderer;
    FontManager * fonts;
    SoundManager * sounds;
    OptionManager * options;

    ThemeManager theme;

    PauseScreen screen_text;

    bool sound_played = false;

    State next_state = State::CREDITS;
public:
    WonState(SDL_Renderer *renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options);
    ~WonState();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer *renderer);

    State getNextState();
};

#endif // WONSTATE_HPP
