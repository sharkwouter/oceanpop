#include "BoardManager.hpp"

BoardManager::BoardManager(int x, int y, int width, int height) : board(width, height) {
    this->start_x = x;
    this->start_y = y;

    this->end_x = GEM_SIZE * this->board.getWidth() + this->start_x;
    this->end_y = GEM_SIZE * this->board.getHeight() + this->start_y;
}

void BoardManager::update() {
}

void BoardManager::draw(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int x = 0; x <= this->board.getWidth(); x++) {
        int current_x = this->start_x + x * GEM_SIZE;
        SDL_RenderDrawLine(
                renderer,
                current_x,
                this->start_x,
                current_x,
                this->end_y
                );
    }
    for (int y = 0; y <= this->board.getHeight(); y++) {
        int current_y = this->start_y + y * GEM_SIZE;
        SDL_RenderDrawLine(
                renderer,
                this->start_y,
                current_y,
                this->end_x,
                current_y
        );
    }

    for (int y = 0; y < this->board.getHeight(); y++) {
        for (int x = 0; x < this->board.getWidth(); x++) {
            switch (this->board.gems[y][x]) {
                case RED:
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    break;
                case ORANGE:
                    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                    break;
                case GREEN:
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                    break;
                case BLUE:
                    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                    break;
                case PURPLE:
                    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
                    break;
                case GRAY:
                    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                    break;
                case BROWN:
                    SDL_SetRenderDrawColor(renderer, 255, 200, 200, 255);
                    break;
                default:
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    break;
            }

            SDL_Rect rect;
            rect.x = GEM_SIZE * x + this->start_x + 1;
            rect.y = GEM_SIZE * y + this->start_y + 1;
            rect.w = GEM_SIZE - 1;
            rect.h = GEM_SIZE - 1;
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}