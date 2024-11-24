#ifndef HOWTOSTATE_HPP
#define HOWTOSTATE_HPP

#include <string>
#include <vector>
#include <SDL.h>
#include "BaseState.hpp"
#include "../ThemeManager.hpp"
#include "../FontManager.hpp"
#include "../SoundManager.hpp"
#include "../OptionManager.hpp"
#include "../PauseScreen.hpp"

class HowToState : public BaseState {
private:
    SDL_Renderer * renderer;
    FontManager * fonts;
    SoundManager * sounds;
    OptionManager * options;

    ThemeManager theme;

    SDL_Texture * text_title = NULL;
    SDL_Texture * text_bottom = NULL;
    SDL_Texture * text_movement = NULL;
    SDL_Texture * text_confirm = NULL;
    SDL_Texture * text_cancel = NULL;
    SDL_Texture * text_menu = NULL;

    std::vector<SDL_Texture*> button_images;

    int text_start_y;

    State next_state = State::MENU;

    void loadTexts();
    void loadImages();
    void updateSizing();
public:
    HowToState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options);
    ~HowToState();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer * renderer);

    State getNextState();
};

#endif // HOWTOSTATE_HPP
