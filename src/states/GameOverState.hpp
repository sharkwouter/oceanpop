#ifndef GAMEOVERSTATE_HPP
#define GAMEOVERSTATE_HPP

#include <SDL.h>

#include <vector>
#include <filesystem>
#include <regex>

#include <json/json.h>

#include "BaseState.hpp"
#include "../BoardManager.hpp"
#include "../ThemeManager.hpp"
#include "../SoundManager.hpp"
#include "../PauseScreen.hpp"

#include "../Shell.hpp"

class GameOverState : public BaseState {
private:
    SDL_Renderer * renderer;
    SoundManager * sounds;
    FontManager * fonts;

    ThemeManager theme;

    PauseScreen screen_text;

    bool sound_played = false;
public:
    GameOverState(SDL_Renderer *renderer, FontManager * fonts, SoundManager * sounds);
    ~GameOverState();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer *renderer);

    State getNextState();
};

#endif // GAMEOVERSTATE_HPP
