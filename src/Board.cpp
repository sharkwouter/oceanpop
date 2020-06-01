#include "Board.hpp"

Board::Board(int starting_point_x, int starting_point_y, int width, int height) {
    this->width = width;
    this->height = height;

    this->gems.reserve(width);
    for (int i = 0; i < width; i++) {
        this->gems.push_back(std::move(std::vector<Gem>()));
        for (int j = 0; j < height; j++) {
            // TODO: Add gems.
        }
    }
}
