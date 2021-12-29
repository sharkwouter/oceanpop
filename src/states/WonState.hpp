#ifndef WONSTATE_HPP
#define WONSTATE_HPP

#include <SDL.h>

#include <vector>

#include "BaseState.hpp"
#include "../BoardManager.hpp"
#include "../ThemeManager.hpp"
#include "../SoundManager.hpp"
#include "../PauseScreen.hpp"

class WonState : public BaseState {
private:
    SDL_Renderer * renderer;
    SoundManager * sounds;
    FontManager * fonts;

    ThemeManager theme;

    PauseScreen screen_text;

    bool sound_played = false;
public:
    WonState(SDL_Renderer *renderer, FontManager * fonts, SoundManager * sounds);
    ~WonState();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer *renderer);

    State getNextState();
};

#endif // WONSTATE_HPP
