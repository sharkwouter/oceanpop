#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include <string>
#include <vector>
#include <SDL.h>
#include "BaseState.hpp"
#include "../ThemeManager.hpp"
#include "../FontManager.hpp"
#include "../SoundManager.hpp"
#include "../OptionManager.hpp"
#include "../PauseScreen.hpp"

class MenuState : public BaseState {
private:
    SDL_Renderer * renderer;
    FontManager * fonts;
    SoundManager * sounds;
    OptionManager * options;

    ThemeManager theme;

    SDL_Texture * text_title;
    std::vector<SDL_Texture*> texts;
    std::vector<SDL_Texture*> sub_texts;

    int text_offset = 2;
    int text_start_y;

    int selection = 0;

    int getOptionY(int number);
public:
    MenuState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options);
    ~MenuState();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer * renderer);

    State getNextState();
};

#endif // MENUSTATE_HPP
