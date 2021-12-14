#include "BoardManager.hpp"

#include <iostream>
#include <string>
#include <cmath>

#include "Sound.hpp"
#include "colors.hpp"

BoardManager::BoardManager(SDL_Renderer *renderer, FontManager *fonts, SoundManager * sounds, int x, int y, int width, int height, int moves, int required_matches, int level, int seed) : fonts(fonts), sounds(sounds) {
    textures.add_texture(image_shells, renderer);

    loadLevel(x, y, width, height, moves, required_matches, level, seed);
}

BoardManager::~BoardManager() {
    delete(this->board);
    SDL_DestroyTexture(text_level);
    SDL_DestroyTexture(text_matches);
    SDL_DestroyTexture(text_moves);
}

void BoardManager::loadLevel(int x, int y, int width, int height, int moves, int required_matches, int level, int seed) {
    if (this->board != NULL) {
        delete(this->board);
    }
    this->board = new Board(width, height, seed);
    this->seed = seed;

    this->rect_board.x = x;
    this->rect_board.y = y;

    this->rect_board.w = SHELL_SIZE * width;
    this->rect_board.h = SHELL_SIZE * height;

    this->selected.x = width / 2;
    this->selected.y = height / 2;

    this->matches = 0;
    this->required_matches = required_matches;
    this->matches_updated = true;

    this->starting_moves = moves;
    this->moves = moves;
    this->moves_updated = true;

    this->level = level;
    this->level_updated = true;

    this->current_action = Action::PICKING;
}

void BoardManager::reset() {
    int board_width = this->board->getWidth();
    int board_height = this->board->getHeight();
    delete(this->board);
    this->board = new Board(board_width, board_height, this->seed);

    this->selected.x = this->board->getWidth() / 2;
    this->selected.y = this->board->getHeight() / 2;

    this->matches = 0;
    this->required_matches = required_matches;
    this->matches_updated = true;

    this->moves = this->starting_moves;
    this->moves_updated = true;

    this->current_action = Action::PICKING;
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
                    this->preview = this->board->getShells();
                    this->current_action = Action::MOVING;
                    sounds->play(Sound::PICK);
                } else if (this->current_action == Action::MOVING) {
                    if (this->board->swap(picked, selected)) {
                        this->current_action = Action::FALLING;
                    } else {
                        this->selected = this->picked;
                        sounds->play(Sound::DROP);
                        this->current_action = Action::PICKING;
                    }
                }
                break;
            case Event::CANCEL:
                if (this->current_action == Action::MOVING) {
                    sounds->play(Sound::DROP);
                    this->selected = this->picked;
                    this->current_action = Action::PICKING;
                }
                break;
            case Event::MOUSEMOVE:
                moveCursorMouse();
                break;
            default:
                break;
        }
    }
}

void BoardManager::moveCursor(int x, int y) {
    SDL_Point newSelected = {this->selected.x + x, this->selected.y + y};

    // Wrap around the screen
    if (newSelected.x < 0) {
        newSelected.x = this->board->getWidth() - 1;
    } else if (newSelected.x >= this->board->getWidth()) {
        newSelected.x = 0;
    } else if (newSelected.y < 0) {
        newSelected.y = this->board->getHeight() - 1;
    } else if (newSelected.y >= this->board->getHeight()) {
        newSelected.y = 0;
    }

    if (this->current_action == Action::MOVING) {
        if (newSelected.x != this->picked.x && newSelected.y != this->picked.y) {
            return;
        }
        // Update preview to draw
        this->preview = this->board->getShellsAfterSwap(this->board->getShells(), this->picked, newSelected);
    }
    this->selected = newSelected;
}

void BoardManager::moveCursorMouse() {
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);
    // Make sure the mouse cursor is on the board
    if (mouse.x > rect_board.x && mouse.x < (rect_board.x + rect_board.w) && mouse.y > rect_board.y && mouse.y < (rect_board.y + rect_board.h)) {
        SDL_Point newSelected = {(mouse.x - rect_board.x)/SHELL_SIZE, (mouse.y - rect_board.y)/SHELL_SIZE};
        if (this->current_action == Action::MOVING) {
            if (newSelected.x != this->picked.x && newSelected.y != this->picked.y) {
                // Snap to the nearest allowed position
                if (abs(newSelected.x-this->picked.x) < abs(newSelected.y-this->picked.y)) {
                    newSelected.x = this->picked.x;
                } else {
                    newSelected.y = this->picked.y;
                }
            }
            // Update preview to draw
            this->preview = this->board->getShellsAfterSwap(this->board->getShells(), this->picked, newSelected);
        }
        this->selected = newSelected;
    }
}

void BoardManager::addMatches(int matches) {
    this->matches += matches;
    matches_updated = true;
}

void BoardManager::increasLevel() {
    level++;
    level_updated = true;
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
            this->board->fillEmpty();
            SDL_Delay(DROP_TIMER);
            if(!this->board->hasEmpty())
                this->current_action = Action::MATCHING;
            break;
        case Action::MATCHING:
            match();
            break;
        default:
            break;
    }

    if (this->required_matches > 0 && this->matches >= this->required_matches && this->current_action == Action::PICKING) {
        this->current_action = Action::COMPLETED;
        this->required_matches += 1;
        increasLevel();
    }
}

void BoardManager::match() {
    SDL_Delay(DROP_TIMER);
    std::vector<Shell> matches = this->board->match();
    if (matches.size() > 0) {
        int scoring_match = 0;
        Sound sound = Sound::MATCH;
        for(Shell match : matches) {
            if (match == Shell::BUBBLE) {
                this->moves++;
            } else if (match == Shell::URCHIN) {
                sound =Sound::PAIN;
                this->matches -= 3;
            } else {
                scoring_match++;
            }
        }
        sounds->play(sound);
        addMatches(scoring_match);
        this->current_action = Action::FALLING;
    } else {
        decreaseMoves();
        this->current_action = Action::PICKING;
    }

}

void BoardManager::draw(SDL_Renderer *renderer) {
    drawBoard(renderer);
    drawCursor(renderer);
    drawInfo(renderer);
    drawShells(renderer);
}

void BoardManager::drawCursor(SDL_Renderer * renderer) {
    // Draw picked cross
    if (this->current_action == Action::MOVING) {
        SDL_Rect pickrect_hor;
        pickrect_hor.x = this->rect_board.x +1;
        pickrect_hor.y = SHELL_SIZE * this->picked.y + this->rect_board.y + 1;
        pickrect_hor.w = SHELL_SIZE * this->board->getWidth();
        pickrect_hor.h = SHELL_SIZE - 1;

        SDL_Rect pickrect_ver;
        pickrect_ver.x = SHELL_SIZE * this->picked.x + this->rect_board.x + 1;
        pickrect_ver.y = this->rect_board.y + 1;
        pickrect_ver.w = SHELL_SIZE - 1;
        pickrect_ver.h =  SHELL_SIZE * this->board->getHeight();

        SDL_SetRenderDrawColor(renderer, COLOR_PICKED.r, COLOR_PICKED.g, COLOR_PICKED.b, COLOR_PICKED.a);
        SDL_RenderFillRect(renderer, &pickrect_ver);
        SDL_RenderFillRect(renderer, &pickrect_hor);
    }

    // Draw selection rectangle
    SDL_Rect selrect;
    selrect.x = SHELL_SIZE * selected.x + this->rect_board.x + 1;
    selrect.y = SHELL_SIZE * selected.y + this->rect_board.y + 1;
    selrect.w = SHELL_SIZE - 1;
    selrect.h = SHELL_SIZE - 1;

    SDL_SetRenderDrawColor(renderer, COLOR_SELECT.r, COLOR_SELECT.g, COLOR_SELECT.b, COLOR_SELECT.a);
    SDL_RenderFillRect(renderer, &selrect);
}

void BoardManager::drawBoard(SDL_Renderer * renderer) {
    // Draw background rectangle
    SDL_Rect background = {this->rect_board.x, this->rect_board.y, this->board->getWidth() * SHELL_SIZE, (this->board->getHeight() + 1) * SHELL_SIZE};
    SDL_SetRenderDrawColor(renderer, COLOR_BOARD.r, COLOR_BOARD.g, COLOR_BOARD.b, COLOR_BOARD.a);
    SDL_RenderFillRect(renderer, &background);

    // Draw board lines
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int x = 0; x <= this->board->getWidth(); x++) {
        int current_x = this->rect_board.x + x * SHELL_SIZE;
        SDL_RenderDrawLine(
                renderer,
                current_x,
                this->rect_board.y,
                current_x,
                (this->rect_board.y + this->rect_board.h)
                );
    }
    for (int y = 0; y <= this->board->getHeight(); y++) {
        int current_y = this->rect_board.y + y * SHELL_SIZE;
        SDL_RenderDrawLine(
                renderer,
                this->rect_board.x,
                current_y,
                (this->rect_board.x + this->rect_board.w),
                current_y
        );
    }

    // Draw lines around scoreboard
    SDL_RenderDrawLine(renderer,this->rect_board.x,(this->rect_board.y + this->rect_board.h) + 1,this->rect_board.x,(this->rect_board.y + this->rect_board.h) + SHELL_SIZE);
    SDL_RenderDrawLine(renderer,(this->rect_board.x + this->rect_board.w),(this->rect_board.y + this->rect_board.h) + 1,(this->rect_board.x + this->rect_board.w),(this->rect_board.y + this->rect_board.h) + SHELL_SIZE );
    SDL_RenderDrawLine(renderer,this->rect_board.x,(this->rect_board.y + this->rect_board.h) + SHELL_SIZE,(this->rect_board.x + this->rect_board.w),(this->rect_board.y + this->rect_board.h) + SHELL_SIZE);
}

void BoardManager::drawShells(SDL_Renderer * renderer) {
    std::vector<std::vector<Shell>> shells = this->board->getShells();
    if (this->current_action == Action::MOVING) {
        shells = this->preview;
    }
    // Draw the shells
    for (int x = 0; x < this->board->getWidth(); x++) {
        for (int y = 0; y < this->board->getHeight(); y++) {
            SDL_Rect srcrect;
            srcrect.x = SHELL_SIZE * (int) shells[x][y];
            srcrect.y = 0;
            srcrect.w = SHELL_SIZE;
            srcrect.h = SHELL_SIZE;

            SDL_Rect dstrect;
            dstrect.x = SHELL_SIZE * x + this->rect_board.x;
            dstrect.y = SHELL_SIZE * y + this->rect_board.y;
            dstrect.w = SHELL_SIZE;
            dstrect.h = SHELL_SIZE;

            SDL_RenderCopy(renderer, textures.get(image_shells), &srcrect, &dstrect);
        }
    }
}

void BoardManager::drawInfo(SDL_Renderer * renderer) {
    // Generate texture with text
    if (level_updated) {
        std::string str_level = std::to_string(level);
        if (this->board->getWidth() > 7) {
            str_level = "level " + str_level;
        } else if (this->board->getWidth() > 3) {
            str_level = "L" + str_level;
        }
        text_level = fonts->getTexture(renderer, str_level, false, {255, 255, 255, 255});
        level_updated = false;
    }
    if (matches_updated) {
        std::string str_matches = std::to_string(matches);
        if (this->required_matches > 0) {
            str_matches += "/" +  std::to_string(required_matches);
        }
        text_matches = fonts->getTexture(renderer, str_matches, false, {255, 255, 255, 255});
        matches_updated = false;
    }
    if (moves_updated) {
        std::string str_moves = std::to_string(this->moves);
        if (this->board->getWidth() > 7) {
            str_moves += " moves";
        } else if (this->board->getWidth() > 3) {
            str_moves += "m";
        }
        text_moves = fonts->getTexture(renderer, str_moves, false, {255, 255, 255, 255});
        moves_updated = false;
    }

    // Render level
    if (level > 0) {
        SDL_Rect rect_level = {rect_board.x, rect_board.y + rect_board.h, 0, 0};
        SDL_QueryTexture(text_level, NULL, NULL, &rect_level.w, &rect_level.h);
        rect_level.x += SHELL_SIZE / 2;
        rect_level.y += SHELL_SIZE / 2 - rect_level.h / 2;
        SDL_RenderCopy(renderer, text_level, NULL, &rect_level);
    }

    // Render matches
    SDL_Rect rect_matches = {rect_board.x + rect_board.w / 2 , rect_board.y + rect_board.h, 0, 0};
    SDL_QueryTexture(text_matches, NULL, NULL, &rect_matches.w, &rect_matches.h);
    rect_matches.x -= rect_matches.w / 2;
    rect_matches.y += SHELL_SIZE / 2 - rect_matches.h / 2;
    SDL_RenderCopy(renderer, text_matches, NULL, &rect_matches);

    // Render moves
    if (this->starting_moves > 0) {
        SDL_Rect rect_moves = {rect_board.x + rect_board.w, rect_board.y + rect_board.h, 0, 0};
        SDL_QueryTexture(text_moves, NULL, NULL, &rect_moves.w, &rect_moves.h);
        rect_moves.x -= SHELL_SIZE / 2 + rect_moves.w;
        rect_moves.y += SHELL_SIZE / 2 - rect_moves.h / 2;
        SDL_RenderCopy(renderer, text_moves, NULL, &rect_moves);
    }
}
