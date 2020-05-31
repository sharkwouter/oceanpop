#ifndef BASESTATE_HPP
#define BASESTATE_HPP

#include "../Board.hpp"

class GameState
{
private:
    Board board;
public:
    GameState();
    ~GameState();

    void update();
    void draw();
};

#endif // BASESTATE_HPP