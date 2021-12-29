#ifndef NOTIMPLEMENTEDSTATE_HPP
#define NOTIMPLEMENTEDSTATE_HPP

#include <SDL.h>

#include <vector>

#include "BaseState.hpp"
#include "../BoardManager.hpp"
#include "../ThemeManager.hpp"
#include "../SoundManager.hpp"
#include "../PauseScreen.hpp"

class NotImplementedState : public BaseState {
private:
    SDL_Renderer * renderer;
    SoundManager * sounds;
    FontManager * fonts;

    ThemeManager theme;

    PauseScreen screen_text;
public:
    NotImplementedState(SDL_Renderer *renderer, FontManager * fonts, SoundManager * sounds);
    ~NotImplementedState();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer *renderer);

    State getNextState();
};

#endif // NOTIMPLEMENTEDSTATE_HPP
