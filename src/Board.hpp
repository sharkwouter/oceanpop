#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include "Gem.hpp"

class Board {
private:
    std::vector<std::vector<Gem>> gems;
public:
    Board(int x, int y, int width, int height);
    ~Board();

    void update();
    void draw();
};

#endif // BOARD_HPP