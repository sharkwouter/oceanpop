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

    SDL_Texture * text_title;
    SDL_Texture * text_bottom;
    std::vector<SDL_Texture*> texts;
    std::vector<SDL_Texture*> images;
    SDL_Texture * slash;


    int text_offset = 2;
    int text_start_y;

    State next_state = State::MENU;

    int getTextY(int number);
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
