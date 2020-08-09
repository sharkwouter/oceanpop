#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include "Gem.hpp"


class Board {
private:
    int width;
    int height;
    std::vector<std::vector<Gem>> gems;
public:
    Board(int starting_point_x, int starting_point_y, int width, int height);

    void update();
    void draw(SDL_Renderer *renderer);
};

#endif // BOARD_HPP
