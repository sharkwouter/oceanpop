#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include <string>
#include <vector>
#include <SDL.h>
#include "BaseState.hpp"
#include "../ThemeManager.hpp"
#include "../FontManager.hpp"
#include "../SoundManager.hpp"
#include "../PauseScreen.hpp"

class MenuState : public BaseState {
private:
    SDL_Renderer * renderer;
    FontManager * fonts;
    SoundManager * sounds;

    ThemeManager theme;

    SDL_Texture * text_title;
    std::vector<SDL_Texture*> options;

    int options_offset = 2;
    int options_start_y;

    int current_option = 0;

    int getOptionY(int number);
public:
    MenuState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds);
    ~MenuState();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer * renderer);

    State getNextState();
};

#endif // MENUSTATE_HPP
