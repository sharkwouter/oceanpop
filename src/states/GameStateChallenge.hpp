#ifndef GAMESTATECHALLENGE_HPP
#define GAMESTATECHALLENGE_HPP

#include <vector>
#include <SDL.h>
#include "BaseState.hpp"
#include "../BoardManager.hpp"
#include "../ThemeManager.hpp"
#include "../SoundManager.hpp"
#include "../FontManager.hpp"
#include "../OptionManager.hpp"
#include "../PauseScreen.hpp"

class GameStateChallenge : public BaseState {
private:
    SDL_Renderer * renderer;
    SoundManager * sounds;
    FontManager * fonts;
    OptionManager * options;

    BoardManager * board;
    ThemeManager theme;

    PauseScreen pause_screen;
    PauseScreen win_screen;
    PauseScreen lose_screen;

    SDL_Texture * text_attempts = NULL;
    SDL_Texture * text_attempts_number = NULL;


    bool paused = false;
    bool completed = false;
    bool failed = false;

    int level;
    int seed;
    int moves;
    int required_matches;
    int width;
    int height;

    int attempts;
    bool attempts_changed;

    State next_state = State::MENU;

    void loadLevel();
public:
    GameStateChallenge(SDL_Renderer *renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options);
    ~GameStateChallenge();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer *renderer);

    State getNextState();
};

#endif // GAMESTATECHALLENGE_HPP
