#include "BoardManager.hpp"

#include <iostream>
#include <string>

BoardManager::BoardManager(SDL_Renderer *renderer, int x, int y, int width, int height) : board(width, height) {
    this->start_x = x;
    this->start_y = y;

    this->end_x = GEM_SIZE * this->board.getWidth() + this->start_x;
    this->end_y = GEM_SIZE * this->board.getHeight() + this->start_y;


    textures.add_texture(image_gems, renderer);
}

void BoardManager::handleEvents(std::vector<Event> events) {
    for (Event e: events) {
        switch (e) {
            case Event::LEFT:
            std::cout << "left" << std::endl;
                break;
            
            default:
                break;
        }
    }
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
            SDL_Rect srcrect;
            srcrect.w = 32;
            srcrect.h = 32;

            switch (this->board.gems[y][x]) {
                case Gem::RED:
                    srcrect.x = 374;
                    srcrect.y = 70;
                    break;
                case Gem::ORANGE:
                    srcrect.x = 374;
                    srcrect.y = 169;
                    break;
                case Gem::GREEN:
                    srcrect.x = 169;
                    srcrect.y = 136;
                    break;
                case Gem::BLUE:
                    srcrect.x = 169;
                    srcrect.y = 169;
                    break;
                case Gem::PURPLE:
                    srcrect.x = 169;
                    srcrect.y = 268;
                    break;
                case Gem::GRAY:
                    srcrect.x = 374;
                    srcrect.y = 202;
                    break;
                case Gem::BROWN:
                    srcrect.x = 169;
                    srcrect.y = 37;
                    break;
                default:
                    srcrect.x = 4;
                    srcrect.y = 4;
                    break;
            }

            SDL_Rect dstrect;
            dstrect.x = GEM_SIZE * x + this->start_x + 1;
            dstrect.y = GEM_SIZE * y + this->start_y + 1;
            dstrect.w = GEM_SIZE - 1;
            dstrect.h = GEM_SIZE - 1;
            SDL_RenderCopy(renderer, textures.get(image_gems), &srcrect, &dstrect);
        }
    }
}
