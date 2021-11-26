#include "BoardManager.hpp"

#include <iostream>
#include <string>

BoardManager::BoardManager(SDL_Renderer *renderer, int x, int y, int width, int height) : board(width, height) {
    this->start_x = x;
    this->start_y = y;

    this->end_x = GEM_SIZE * this->board.getWidth() + this->start_x;
    this->end_y = GEM_SIZE * this->board.getHeight() + this->start_y;

    this->selected.x = width / 2;
    this->selected.y = height / 2;

    textures.add_texture(image_gems, renderer);

    this->current_action = Action::FALLING;
}

void BoardManager::handleEvents(std::vector<Event> events) {
    if (this->current_action == Action::MATCHING || this->current_action == Action::FALLING)
        return;
    
    for (Event e: events) {
        switch (e) {
            case Event::LEFT:
                moveCursor(-1, 0);
                break;
            case Event::RIGHT:
                moveCursor(1, 0);
                break;
            case Event::UP:
                moveCursor(0, -1);
                break;
            case Event::DOWN:
                moveCursor(0, 1);
                break;
            case Event::CONFIRM:
                if (this->current_action == Action::PICKING) {
                    this->picked = this->selected;
                    this->current_action = Action::MOVING;
                } else if (this->current_action == Action::MOVING) {
                    if (this->board.swap(picked, selected)) {
                        this->current_action = Action::FALLING;
                    } else {
                        this->selected = this->picked;
                        this->current_action = Action::PICKING;
                    }
                        
                }
                break;
            case Event::CANCEL:
                if (this->current_action == Action::MOVING) {
                    this->selected = this->picked;
                    this->current_action = Action::PICKING;
                }
            default:
                break;
        }
    }
}

void BoardManager::moveCursor(int x, int y) {
    SDL_Point newSelected = {this->selected.x + x, this->selected.y + y};

    // Wrap around the screen
    if (newSelected.x < 0) {
        newSelected.x = this->board.getWidth() - 1;
    } else if (newSelected.x >= this->board.getWidth()) {
        newSelected.x = 0;
    } else if (newSelected.y < 0) {
        newSelected.y = this->board.getHeight() - 1;
    } else if (newSelected.y >= this->board.getHeight()) {
        newSelected.y = 0;
    }

    if (this->current_action == Action::MOVING) {
        if (newSelected.x != this->picked.x && newSelected.y != this->picked.y) {
            return;
        }
    }

    this->selected = newSelected;
}

void BoardManager::update() {
    switch (this->current_action) {
        case Action::FALLING:
            this->board.fillEmpty();
            SDL_Delay(DROP_TIMER);
            if(!this->board.hasEmpty())
                this->current_action = Action::MATCHING;
            break;
        case Action::MATCHING:
            SDL_Delay(DROP_TIMER);
            int score = this->board.match();
            if (score > 0) {
                this->current_action = Action::FALLING;
            } else {
                this->current_action = Action::PICKING;
            }
            break;
    }
}

void BoardManager::draw(SDL_Renderer *renderer) {
    // Draw selection rectangle
    SDL_Rect selrect;
    selrect.x = GEM_SIZE * selected.x + this->start_x;
    selrect.y = GEM_SIZE * selected.y + this->start_y;
    selrect.w = GEM_SIZE;
    selrect.h = GEM_SIZE;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &selrect);

    // Draw picked rectangle
    if (this->current_action == Action::MOVING) {
        SDL_Rect pickrect;
        pickrect.x = GEM_SIZE * picked.x + this->start_x;
        pickrect.y = GEM_SIZE * picked.y + this->start_y;
        pickrect.w = GEM_SIZE;
        pickrect.h = GEM_SIZE;

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &pickrect);
    }

    // Draw board lines
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

    // Draw the gems
    for (int x = 0; x < this->board.getWidth(); x++) {
        for (int y = 0; y < this->board.getHeight(); y++) {
            SDL_Rect srcrect;
            srcrect.w = 32;
            srcrect.h = 32;

            switch (this->board.getGems()[x][y]) {
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
                    continue;
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
