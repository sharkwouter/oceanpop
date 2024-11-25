#ifndef HOWTOPLAYSTATE_HPP
#define HOWTOPLAYSTATE_HPP

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

class HowToPlayState : public BaseState {
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
    SDL_Texture * text_match_bubbles = NULL;
    SDL_Texture * text_match_urchins = NULL;
    SDL_Texture * text_goal = NULL;

    int text_start_y;

    State next_state = State::HOWTOMATCH;

    void loadTexts();
    void updateSizing();
public:
    HowToPlayState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options);
    ~HowToPlayState();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer * renderer);

    State getNextState();
};

#endif // HOWTOPLAYSTATE_HPP
