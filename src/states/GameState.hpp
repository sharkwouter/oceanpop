#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <vector>
#include <SDL.h>
#include "BaseState.hpp"
#include "../BoardManager.hpp"
#include "../ThemeManager.hpp"

class GameState : public BaseState {
private:
    BoardManager board;
    ThemeManager theme;
    FontManager fonts;

    SDL_Texture * text_paused = NULL;
    SDL_Texture * text_paused_subtitle = NULL;
    
    SDL_Texture * text_completed = NULL;
    SDL_Texture * text_completed_subtitle = NULL;

    bool paused = false;
    bool completed = false;

    void drawPauseScreen(SDL_Renderer * renderer);
    void drawCompletedScreen(SDL_Renderer * renderer);
public:
    GameState(SDL_Renderer *renderer);
    ~GameState();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer *renderer);

    State getNextState();
};

#endif // GAMESTATE_HPP
