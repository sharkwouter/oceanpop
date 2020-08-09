#include "Board.hpp"
#include "constants.hpp"

Board::Board(int starting_point_x, int starting_point_y, int width, int height) {
    this->width = width;
    this->height = height;
    this->starting_point_x = starting_point_x;
    this->starting_point_y = starting_point_y;

    this->gems.reserve(width);
    for (int i = 0; i < width; i++) {
        this->gems.push_back(std::move(std::vector<Gem>()));
        this->gems[i].reserve(height);
        for (int j = 0; j < height; j++) {
            // TODO: Add gems.
        }
    }

    this->end_x = GEM_SIZE * this->width + this->starting_point_x;
    this->end_y = GEM_SIZE * this->height + this->starting_point_y;
}

void Board::draw(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int x = 0; x <= this->width; x++) {
        int current_x = this->starting_point_x + x * GEM_SIZE;
        SDL_RenderDrawLine(
                renderer,
                current_x,
                this->starting_point_x,
                current_x,
                this->end_y
                );
    }
    for (int y = 0; y <= this->height; y++) {
        int current_y = this->starting_point_y + y * GEM_SIZE;
        SDL_RenderDrawLine(
                renderer,
                this->starting_point_y,
                current_y,
                this->end_x,
                current_y
        );
    }
}

void Board::update() {

}