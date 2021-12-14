#ifndef GAMESTATERELAXED_HPP
#define GAMESTATERELAXED_HPP

#include <SDL.h>
#include <vector>
#include "BaseState.hpp"
#include "../BoardManager.hpp"
#include "../ThemeManager.hpp"
#include "../SoundManager.hpp"
#include "../PauseScreen.hpp"

class GameStateRelaxed : public BaseState {
private:
    SDL_Renderer * renderer;
    SoundManager * sounds;
    FontManager * fonts;

    BoardManager * board;
    ThemeManager theme;

    PauseScreen pause_screen;

    bool paused = false;

    int seed;
    int width;
    int height;
    SDL_Point position;

    SDL_Point calculatePosition(int width, int height);
    void loadLevel();
public:
    GameStateRelaxed(SDL_Renderer *renderer, FontManager * fonts, SoundManager * sounds);
    ~GameStateRelaxed();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer *renderer);

    State getNextState();
};

#endif // GAMESTATERELAXED_HPP
