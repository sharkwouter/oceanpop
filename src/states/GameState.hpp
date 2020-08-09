#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "BaseState.hpp"
#include "../Board.hpp"

class GameState : public BaseState {
private:
    Board board;
public:
    GameState();
    ~GameState();

    void update();
    void draw();
};

#endif // GAMESTATE_HPP
