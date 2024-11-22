#ifndef GAMESTATERELAXED_HPP
#define GAMESTATERELAXED_HPP

#include <SDL.h>
#include <vector>
#include "BaseState.hpp"
#include "../BoardManager.hpp"
#include "../ThemeManager.hpp"
#include "../SoundManager.hpp"
#include "../FontManager.hpp"
#include "../OptionManager.hpp"
#include "../PauseScreen.hpp"

class GameStateRelaxed : public BaseState {
private:
    SDL_Renderer * renderer;
    SoundManager * sounds;
    FontManager * fonts;
    OptionManager * options;

    BoardManager * board;
    ThemeManager theme;

    PauseScreen pause_screen;

    bool paused = false;

    int seed;
    int width;
    int height;

    State next_state = State::MENU;

    void loadLevel();
public:
    GameStateRelaxed(SDL_Renderer *renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options);
    ~GameStateRelaxed();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer *renderer);

    State getNextState();
};

#endif // GAMESTATERELAXED_HPP
