#include "BoardManager.hpp"

#include <iostream>
#include <string>

BoardManager::BoardManager(SDL_Renderer *renderer, int x, int y, int width, int height) : board(width, height) {
    this->start_x = x;
    this->start_y = y;

    this->end_x = this->start_x + GEM_SIZE * this->board.getWidth();
    this->end_y = this->start_y + GEM_SIZE * this->board.getHeight();

    this->selected.x = width / 2;
    this->selected.y = height / 2;

    fonts.load();

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
    drawBoard(renderer);
    drawCursor(renderer);
    drawScore(renderer);
    drawGems(renderer);
}

void BoardManager::drawCursor(SDL_Renderer * renderer) {
    // Draw picked rectangle
    if (this->current_action == Action::MOVING) {
        SDL_Rect pickrect;
        pickrect.x = GEM_SIZE * picked.x + this->start_x + 1;
        pickrect.y = GEM_SIZE * picked.y + this->start_y + 1;
        pickrect.w = GEM_SIZE - 1;
        pickrect.h = GEM_SIZE - 1;

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &pickrect);
    }

    // Draw selection rectangle
    SDL_Rect selrect;
    selrect.x = GEM_SIZE * selected.x + this->start_x + 1;
    selrect.y = GEM_SIZE * selected.y + this->start_y + 1;
    selrect.w = GEM_SIZE - 1;
    selrect.h = GEM_SIZE - 1;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &selrect);
}

void BoardManager::drawBoard(SDL_Renderer * renderer) {
    // Draw background rectangle
    SDL_Rect background = {this->start_x, this->start_y, this->board.getWidth() * GEM_SIZE, (this->board.getHeight() + 1) * GEM_SIZE};
    SDL_SetRenderDrawColor(renderer, 51, 153, 255, 128);
    SDL_RenderFillRect(renderer, &background);

    // Draw board lines
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int x = 0; x <= this->board.getWidth(); x++) {
        int current_x = this->start_x + x * GEM_SIZE;
        SDL_RenderDrawLine(
                renderer,
                current_x,
                this->start_y,
                current_x,
                this->end_y
                );
    }
    for (int y = 0; y <= this->board.getHeight(); y++) {
        int current_y = this->start_y + y * GEM_SIZE;
        SDL_RenderDrawLine(
                renderer,
                this->start_x,
                current_y,
                this->end_x,
                current_y
        );
    }

    // Draw lines around scoreboard
    SDL_RenderDrawLine(renderer,this->start_x,this->end_y + 1,this->start_x,this->end_y + GEM_SIZE);
    SDL_RenderDrawLine(renderer,this->end_x,this->end_y + 1,this->end_x,this->end_y + GEM_SIZE );
    SDL_RenderDrawLine(renderer,this->start_x,this->end_y + GEM_SIZE,this->end_x,this->end_y + GEM_SIZE);
}

void BoardManager::drawGems(SDL_Renderer * renderer) {
    // Draw the gems
    for (int x = 0; x < this->board.getWidth(); x++) {
        for (int y = 0; y < this->board.getHeight(); y++) {
            int style = 0;
            SDL_Color color;
            switch (this->board.getGems()[x][y]) {
                case Gem::RED:
                    style = 0;
                    color = {230, 0, 0, 255};
                    break;
                case Gem::ORANGE:
                    style = 2;
                    color = {255, 128, 0, 255};
                    break;
                case Gem::GREEN:
                    style = 5;
                    color = {0, 153, 0, 255};
                    break;
                case Gem::BLUE:
                    style = 4;
                    color = {0, 102, 204, 255};
                    break;
                case Gem::PURPLE:
                    style = 2;
                    color = {153, 0, 153, 255};
                    break;
                case Gem::GRAY:
                    style = 1;
                    color = {192, 192, 192, 255};
                    break;
                case Gem::BROWN:
                    style = 3;
                    color = {153, 76, 0, 255};
                    break;
                default:
                    continue;
                    break;
            }

            SDL_Rect srcrect;
            srcrect.x = 32 * style;
            srcrect.y = 0;
            srcrect.w = 32;
            srcrect.h = 32;

            SDL_Rect dstrect;
            dstrect.x = GEM_SIZE * x + this->start_x + 5;
            dstrect.y = GEM_SIZE * y + this->start_y + 5;
            dstrect.w = GEM_SIZE - 10;
            dstrect.h = GEM_SIZE - 10;

            SDL_SetTextureColorMod(textures.get(image_gems), color.r, color.g, color.b);
            SDL_RenderCopy(renderer, textures.get(image_gems), &srcrect, &dstrect);
        }
    }
}

void BoardManager::drawScore(SDL_Renderer * renderer) {
    // Generate texture with text
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Texture * text_moves = fonts.getTexture(renderer, "5 moves left", false, textColor);
    SDL_Texture * text_score = fonts.getTexture(renderer, "50/1500", false, textColor);

    // Render moves
    SDL_Rect rect_moves = {start_x, end_y, 0, 0};
    SDL_QueryTexture(text_moves, NULL, NULL, &rect_moves.w, &rect_moves.h);
    rect_moves.x += GEM_SIZE * 2 - rect_moves.w/2;
    rect_moves.y += GEM_SIZE/2 - rect_moves.h/2;
    SDL_RenderCopy(renderer, text_moves, NULL, &rect_moves);

    // Render score
    SDL_Rect rect_score = {end_x, end_y, 0, 0};
    SDL_QueryTexture(text_score, NULL, NULL, &rect_score.w, &rect_score.h);
    rect_score.x -= GEM_SIZE * 2 + rect_score.w/2;
    rect_score.y += GEM_SIZE/2 - rect_score.h/2;
    SDL_RenderCopy(renderer, text_score, NULL, &rect_score);
}
