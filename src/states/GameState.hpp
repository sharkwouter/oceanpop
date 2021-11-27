#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <vector>
#include <SDL.h>
#include "BaseState.hpp"
#include "../BoardManager.hpp"

class GameState : public BaseState {
private:
    BoardManager board;
    TextureManager textures;

    std::string background_image = "assets/backgrounds/background1.jpg";
public:
    GameState(SDL_Renderer *renderer);
    ~GameState();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer *renderer);
};

#endif // GAMESTATE_HPP
