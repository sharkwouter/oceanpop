#include "BoardManager.hpp"

#include <iostream>
#include <string>

#include "Sound.hpp"

BoardManager::BoardManager(SDL_Renderer *renderer, int x, int y, int width, int height) : board(width, height) {
    this->start.x = x;
    this->start.y = y;

    this->end.x = this->start.x + GEM_SIZE * this->board.getWidth();
    this->end.y = this->start.y + GEM_SIZE * this->board.getHeight();

    this->selected.x = width / 2;
    this->selected.y = height / 2;

    sounds.load();
    fonts.load();

    textures.add_texture(image_shells, renderer);

    init();
}

void BoardManager::init() {
    this->score = 0;
    this->moves = 10;
    this->required_score = 300;
    this->current_action = Action::PICKING;
}

void BoardManager::reset() {
    this->board = Board(this->board.getWidth(), this->board.getHeight());
    init();
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
                    this->preview = this->board.getShells();
                    this->current_action = Action::MOVING;
                    sounds.play(Sound::PICK);
                } else if (this->current_action == Action::MOVING) {
                    if (this->board.swap(picked, selected)) {
                        this->current_action = Action::FALLING;
                    } else {
                        this->selected = this->picked;
                        sounds.play(Sound::DROP);
                        this->current_action = Action::PICKING;
                    }
                        
                }
                break;
            case Event::CANCEL:
                if (this->current_action == Action::MOVING) {
                    sounds.play(Sound::DROP);
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
        // Update preview to draw
        this->preview = this->board.getShellsAfterSwap(this->board.getShells(), this->picked, newSelected);
    }

    this->selected = newSelected;
}

void BoardManager::addScore(int matches) {
    this->score += 10*matches*(matches+1)/2;
    score_updated = true;
}

void BoardManager::increaseMoves() {
    moves++;
    moves_updated = true;
}

void BoardManager::decreaseMoves() {
    moves--;
    moves_updated = true;
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
            match();
            break;
    }
}

void BoardManager::match() {
    SDL_Delay(DROP_TIMER);
    std::vector<Shell> matches = this->board.match();
    if (matches.size() > 0) {
        int scoring_match = 0;
        Sound sound = Sound::MATCH;
        for(Shell match : matches) {
            if (match == Shell::BUBBLE) {
                this->moves++;
            } else if (match == Shell::URCHIN) {
                sound =Sound::PAIN;
                this->score -= 50;
            } else {
                scoring_match++;
            }
        }
        sounds.play(sound);
        addScore(scoring_match);
        this->current_action = Action::FALLING;
    } else {
        decreaseMoves();
        this->current_action = Action::PICKING;
    }

}

void BoardManager::draw(SDL_Renderer *renderer) {
    drawBoard(renderer);
    drawCursor(renderer);
    drawScore(renderer);
    drawShells(renderer);
}

void BoardManager::drawCursor(SDL_Renderer * renderer) {
    // Draw picked cross
    if (this->current_action == Action::MOVING) {
        SDL_Rect pickrect_hor;
        pickrect_hor.x = this->start.x +1;
        pickrect_hor.y = GEM_SIZE * this->picked.y + this->start.y + 1;
        pickrect_hor.w = GEM_SIZE * this->board.getWidth();
        pickrect_hor.h = GEM_SIZE - 1;

        SDL_Rect pickrect_ver;
        pickrect_ver.x = GEM_SIZE * this->picked.x + this->start.x + 1;
        pickrect_ver.y = this->start.y + 1;
        pickrect_ver.w = GEM_SIZE - 1;
        pickrect_ver.h =  GEM_SIZE * this->board.getHeight();

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 50);
        SDL_RenderFillRect(renderer, &pickrect_ver);
        SDL_RenderFillRect(renderer, &pickrect_hor);
    }

    // Draw selection rectangle
    SDL_Rect selrect;
    selrect.x = GEM_SIZE * selected.x + this->start.x + 1;
    selrect.y = GEM_SIZE * selected.y + this->start.y + 1;
    selrect.w = GEM_SIZE - 1;
    selrect.h = GEM_SIZE - 1;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
    SDL_RenderFillRect(renderer, &selrect);
}

void BoardManager::drawBoard(SDL_Renderer * renderer) {
    // Draw background rectangle
    SDL_Rect background = {this->start.x, this->start.y, this->board.getWidth() * GEM_SIZE, (this->board.getHeight() + 1) * GEM_SIZE};
    SDL_SetRenderDrawColor(renderer, 51, 153, 255, 128);
    SDL_RenderFillRect(renderer, &background);

    // Draw board lines
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int x = 0; x <= this->board.getWidth(); x++) {
        int current_x = this->start.x + x * GEM_SIZE;
        SDL_RenderDrawLine(
                renderer,
                current_x,
                this->start.y,
                current_x,
                this->end.y
                );
    }
    for (int y = 0; y <= this->board.getHeight(); y++) {
        int current_y = this->start.y + y * GEM_SIZE;
        SDL_RenderDrawLine(
                renderer,
                this->start.x,
                current_y,
                this->end.x,
                current_y
        );
    }

    // Draw lines around scoreboard
    SDL_RenderDrawLine(renderer,this->start.x,this->end.y + 1,this->start.x,this->end.y + GEM_SIZE);
    SDL_RenderDrawLine(renderer,this->end.x,this->end.y + 1,this->end.x,this->end.y + GEM_SIZE );
    SDL_RenderDrawLine(renderer,this->start.x,this->end.y + GEM_SIZE,this->end.x,this->end.y + GEM_SIZE);
}

void BoardManager::drawShells(SDL_Renderer * renderer) {
    std::vector<std::vector<Shell>> shells = this->board.getShells();
    if (this->current_action == Action::MOVING) {
        shells = this->preview;
    }
    // Draw the shells
    for (int x = 0; x < this->board.getWidth(); x++) {
        for (int y = 0; y < this->board.getHeight(); y++) {
            SDL_Rect srcrect;
            srcrect.x = GEM_SIZE * (int) shells[x][y];
            srcrect.y = 0;
            srcrect.w = GEM_SIZE;
            srcrect.h = GEM_SIZE;

            SDL_Rect dstrect;
            dstrect.x = GEM_SIZE * x + this->start.x;
            dstrect.y = GEM_SIZE * y + this->start.y;
            dstrect.w = GEM_SIZE;
            dstrect.h = GEM_SIZE;

            SDL_RenderCopy(renderer, textures.get(image_shells), &srcrect, &dstrect);
        }
    }
}

void BoardManager::drawScore(SDL_Renderer * renderer) {
    // Generate texture with text
    if (moves_updated) {
        text_moves = fonts.getTexture(renderer, std::to_string(moves) + " moves left", false, {255, 255, 255, 255});
        moves_updated = false;
    }
    if (score_updated) {
        text_score = fonts.getTexture(renderer, std::to_string(score) + "/" + std::to_string(required_score), false, {255, 255, 255, 255});
        score_updated = false;
    }

    // Render moves
    SDL_Rect rect_moves = {start.x, end.y, 0, 0};
    SDL_QueryTexture(text_moves, NULL, NULL, &rect_moves.w, &rect_moves.h);
    rect_moves.x += GEM_SIZE * 2 - rect_moves.w/2;
    rect_moves.y += GEM_SIZE/2 - rect_moves.h/2;
    SDL_RenderCopy(renderer, text_moves, NULL, &rect_moves);

    // Render score
    SDL_Rect rect_score = {end.x, end.y, 0, 0};
    SDL_QueryTexture(text_score, NULL, NULL, &rect_score.w, &rect_score.h);
    rect_score.x -= GEM_SIZE * 2 + rect_score.w/2;
    rect_score.y += GEM_SIZE/2 - rect_score.h/2;
    SDL_RenderCopy(renderer, text_score, NULL, &rect_score);
}
