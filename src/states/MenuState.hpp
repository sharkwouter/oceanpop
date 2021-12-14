#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include <string>
#include <vector>
#include <SDL.h>
#include "BaseState.hpp"
#include "../ThemeManager.hpp"
#include "../FontManager.hpp"
#include "../SoundManager.hpp"
#include "../MenuOption.hpp"

class MenuState : public BaseState {
private:
    SDL_Renderer * renderer;
    FontManager * fonts;
    SoundManager * sounds;

    ThemeManager theme;
    std::string background = "assets/backgrounds/background1.jpg";

    SDL_Texture * text_title;
    std::vector<SDL_Texture*> options;

    int current_option = 0;
public:
    MenuState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds);
    ~MenuState();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer * renderer);

    State getNextState();
};

#endif // MENUSTATE_HPP
