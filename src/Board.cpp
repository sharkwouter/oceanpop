#include "Board.hpp"

Board::Board(int width, int height) {
    this->gems.reserve(height);
    for (int y = 0; y < height; y++) {
        this->gems.push_back(std::move(std::vector<Gem>()));
        this->gems[y].reserve(width);
        for (int x = 0; x < width; x++) {
            // TODO: Add gems.
            this->gems[y].push_back((Gem) (rand() % (int) Gem::NUMBER_OF_COLORS));
        }
    }
}

int Board::getHeight() {
    return gems.size();
}

int Board::getWidth() {
    return gems[0].size();
}
