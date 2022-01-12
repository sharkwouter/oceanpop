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

BoardManager::BoardManager(SDL_Renderer *renderer, FontManager *fonts, SoundManager * sounds, int x, int y, std::vector<std::vector<ShellType>> shells, int moves, int required_matches, int level, int seed) : fonts(fonts), sounds(sounds) {
    textures.add_texture(image_shells, renderer);
    loadLevel(x, y, shells, moves, required_matches, level, seed);
}

BoardManager::~BoardManager() {
    delete(this->board);
    SDL_DestroyTexture(text_level);
    SDL_DestroyTexture(text_matches);
    SDL_DestroyTexture(text_moves);
}

void BoardManager::loadLevel(int x, int y, int width, int height, int moves, int required_matches, int level, int seed) {
    this->seed = seed;
    storeLevel(x, y, new Board(width, height, seed), moves, required_matches, level);
    this->preview = this->board->getShells();
    this->current_action = Action::PICKING;
}

void BoardManager::loadLevel(int x, int y, std::vector<std::vector<ShellType>> shells, int moves, int required_matches, int level, int seed) {
    this->starting_shells = shells;
    this->seed = seed;
    storeLevel(x, y, new Board(shells, seed), moves, required_matches, level);
    this->preview = this->board->getShells();
    this->current_action = Action::PICKING;
}

void BoardManager::storeLevel(int x, int y, Board * board, int moves, int required_matches, int level) {
    if (this->board != NULL) {
        delete(this->board);
    }
    this->board = std::move(board);

    this->rect_board.x = x;
    this->rect_board.y = y;

    this->rect_board.w = SHELL_SIZE * this->board->getWidth();
    this->rect_board.h = SHELL_SIZE * this->board->getHeight();

    this->selected.x = this->board->getWidth() / 2;
    this->selected.y = this->board->getHeight() / 2;

    this->matches = 0;
    this->required_matches = required_matches;
    if (this->required_matches == 0) {
        this->isRelaxedMode = true;
    }

    this->matches_updated = true;

    this->moves = moves;
    this->starting_moves = this->moves;
    this->moves_updated = true;

    this->level = level;
    this->level_updated = true;
}

void BoardManager::reset() {
    int width = this->board->getWidth();
    int height = this->board->getHeight();
    delete(this->board);
    if ((int) this->starting_shells.size() > 0) {
        this->board = new Board(this->starting_shells, this->seed);
    } else {
        this->board = new Board(width, height, this->seed);
    }

    this->selected.x = this->board->getWidth() / 2;
    this->selected.y = this->board->getHeight() / 2;

    this->matches = 0;
    this->matches_updated = true;

    this->moves = this->starting_moves;
    this->moves_updated = true;

    this->preview = this->board->getShells();
    this->current_action = Action::PICKING;
}

void BoardManager::handleEvents(std::vector<Event> events) {
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
                    sounds->play(Sound::PICK);
                } else if (this->current_action == Action::MOVING) {
                    if (this->board->swap(picked, selected)) {
                        this->current_action = Action::FALLING_START;
                    } else {
                        this->selected = this->picked;
                        this->preview = this->board->getShells();
                        sounds->play(Sound::DROP);
                        this->current_action = Action::PICKING;
                    }
                }
                break;
            case Event::CANCEL:
                if (this->current_action == Action::MOVING) {
                    sounds->play(Sound::DROP);
                    this->selected = this->picked;
                    this->preview = this->board->getShells();
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
    if (!this->bubbles_matched) {
        this->moves--;
    } else {
        this->bubbles_matched = false;
    }
    this->moves_updated = true;
}

void BoardManager::update() {
    int delta;

    switch (this->current_action) {
        case Action::PICKING:
        case Action::MOVING:
            break;
        case Action::MATCHING_START:
            match();
            if (matches_made.size() > 0) {
                this->animation = 0;
                this->preview = this->board->getShells();
                this->animation_start = SDL_GetTicks();
                this->current_action = Action::ANIMATE_MATCHING;
            } else {
                this->current_action = Action::MATCHING_END;
            }
            break;
        case Action::ANIMATE_MATCHING:
            delta = SDL_GetTicks() - this->animation_start;
            if(delta >= MATCH_DELAY) {
                this->animation += delta/MATCH_DELAY;
                this->animation_start = SDL_GetTicks();
            }
            if (this->animation > MATCH_STEPS) {
                this->matches_updated = true;
                this->moves_updated = true;
                this->current_action = Action::FALLING_START;
            }
            break;
        case Action::MATCHING_END:
            decreaseMoves();
            if (!this->isRelaxedMode && this->matches >= this->required_matches) {
                this->current_action = Action::COMPLETED;
                this->required_matches += 1;
                increasLevel();
            } else {
                this->current_action = Action::PICKING;
            }
            break;
        case Action::FALLING_START:
            this->preview = this->board->getShells();
            if(!this->board->hasEmpty())
                this->current_action = Action::FALLING_END;
            else {
                this->animation = 0;
                this->shells_to_drop = this->board->dropShells();
                this->animation_start = SDL_GetTicks();
                this->current_action = Action::ANIMATE_FALLING;
            }
            break;
        case Action::ANIMATE_FALLING:
            delta = SDL_GetTicks() - this->animation_start;
            if(delta >= DROP_DELAY) {
                this->animation += delta/DROP_DELAY;
                this->animation_start = SDL_GetTicks();
            }
            if (this->animation > DROP_STEPS) {
                this->current_action = Action::FALLING_START;
                this->preview = this->board->getShells();
            }
            break;
        case Action::FALLING_END:
            this->preview = this->board->getShells();
            this->current_action = Action::MATCHING_START;
            break;
        case Action::COMPLETED:
            // Do not add code here
            // We're waiting for the game state to clean us up
            break;
    }
}

void BoardManager::match() {
    this->matches_made = this->board->match();
    if (this->matches_made.size() > 0) {
        int scoring_matches = 0;
        Sound sound = Sound::MATCH;
        for(Match match : matches_made) {
            if (match.type == ShellType::BUBBLE) {
                if (!this->isRelaxedMode) {
                    this->bubbles_matched = true;
                    this->moves++;
                } else {
                    this->matches += 3;
                }
            } else if (match.type == ShellType::URCHIN) {
                sound =Sound::PAIN;
                this->matches -= 3;
            } else {
                scoring_matches++;
            }
        }
        sounds->play(sound);
        this->matches += scoring_matches;
    }
}

bool BoardManager::isFalling(SDL_Point point) {
    for (Shell falling: this->shells_to_drop) {
        if (falling.x == point.x && falling.y == point.y) {
            return true;
        }
    }
    return false;
}

void BoardManager::draw(SDL_Renderer *renderer) {
    drawBoard(renderer);
    drawCursor(renderer);
    drawInfo(renderer);
    drawShells(renderer);

    if (this->current_action == Action::ANIMATE_FALLING) {
        drawFallingShells(renderer);
    } else if(this->current_action == Action::ANIMATE_MATCHING) {
        drawMatches(renderer);
    }
}

void BoardManager::drawCursor(SDL_Renderer * renderer) {
    // Draw picked cross
    if (this->current_action == Action::MOVING) {
        SDL_Rect pickrect_hor;
        pickrect_hor.x = this->rect_board.x + 1;
        pickrect_hor.y = SHELL_SIZE * this->picked.y + this->rect_board.y + 1;
        pickrect_hor.w = SHELL_SIZE * this->board->getWidth() - 1;
        pickrect_hor.h = SHELL_SIZE - 1;

        SDL_Rect pickrect_ver;
        pickrect_ver.x = SHELL_SIZE * this->picked.x + this->rect_board.x + 1;
        pickrect_ver.y = this->rect_board.y + 1;
        pickrect_ver.w = SHELL_SIZE - 1;
        pickrect_ver.h =  SHELL_SIZE * this->board->getHeight() - 1;

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
    // Draw the shells
    for (int x = 0; x < this->board->getWidth(); x++) {
        for (int y = 0; y < this->board->getHeight(); y++) {
            if (this->current_action == Action::ANIMATE_FALLING && isFalling({x,y})) {
                continue;
            }

            SDL_Rect srcrect;
            srcrect.x = SHELL_SIZE * (int) this->preview[x][y];
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

void BoardManager::drawFallingShells(SDL_Renderer * renderer) {   
    // Draw the shells
    for(Shell shell : this->shells_to_drop) {
        SDL_Rect srcrect;
        srcrect.x = SHELL_SIZE * (int) shell.type;
        srcrect.y = 0;
        srcrect.w = SHELL_SIZE;
        srcrect.h = SHELL_SIZE;

        SDL_Rect dstrect;
        dstrect.x = SHELL_SIZE * shell.x + this->rect_board.x;
        dstrect.y = SHELL_SIZE * shell.y + this->rect_board.y + animation;
        dstrect.w = SHELL_SIZE;
        dstrect.h = SHELL_SIZE;

        if (shell.y == -1) {
            int start_y = this->rect_board.y - dstrect.y;
            dstrect.y = this->rect_board.y;
            dstrect.h = SHELL_SIZE - start_y;
            srcrect.y = start_y;
            srcrect.h = dstrect.h;
        }

        SDL_RenderCopy(renderer, textures.get(image_shells), &srcrect, &dstrect);
    }
}

void BoardManager::drawMatches(SDL_Renderer * renderer) {
    if (this->text_plus_one == NULL) {
        this->text_plus_one = fonts->getTexture(renderer, "+1", false, {255, 255, 255, 255});
    }
    if (this->text_minus_three == NULL) {
        this->text_minus_three = fonts->getTexture(renderer, "-3", false, {0, 0, 0, 255});
    }

    for(Match match : this->matches_made) {
        if (match.type != ShellType::BUBBLE) {
            // Draw the score text
            SDL_Texture * text = text_plus_one;
            if (match.type == ShellType::URCHIN) {
                text = text_minus_three;
            }

            SDL_Rect rect_plus_one = {SHELL_SIZE * match.x + this->rect_board.x, SHELL_SIZE * match.y + this->rect_board.y, 0, 0};
            SDL_QueryTexture(text, NULL, NULL, &rect_plus_one.w, &rect_plus_one.h);
            rect_plus_one.x += SHELL_SIZE/2 - rect_plus_one.w/2;
            rect_plus_one.y += SHELL_SIZE/2 - rect_plus_one.h/2;
            if (match.direction == Direction::HORIZONTAL) {
                rect_plus_one.x += SHELL_SIZE;
            } else {
                rect_plus_one.y += SHELL_SIZE;
            }

            SDL_RenderCopy(renderer, text, NULL, &rect_plus_one);
        }

        // Draw the shells
        for(int i = 0; i < 3; i++) {
            SDL_Rect srcrect;
            srcrect.x = SHELL_SIZE * (int) match.type;
            srcrect.y = 0;
            srcrect.w = SHELL_SIZE;
            srcrect.h = SHELL_SIZE;

            SDL_Rect dstrect;
            dstrect.x = SHELL_SIZE * match.x + this->rect_board.x;
            dstrect.y = SHELL_SIZE * match.y + this->rect_board.y;
            dstrect.w = SHELL_SIZE;
            dstrect.h = SHELL_SIZE;

            if (match.direction == Direction::HORIZONTAL) {
                dstrect.x += SHELL_SIZE*i;
            } else {
                dstrect.y += SHELL_SIZE*i;
            }

            // Move towards score/moves
            if (match.type == ShellType::BUBBLE && !this->isRelaxedMode) {
                dstrect.x += (rect_moves.x-dstrect.x)/MATCH_STEPS*animation;
                dstrect.y += (rect_moves.y-dstrect.y)/MATCH_STEPS*animation;
            } else {
                dstrect.x += (rect_matches.x-dstrect.x)/MATCH_STEPS*animation;
                dstrect.y += (rect_matches.y-dstrect.y)/MATCH_STEPS*animation;
            }

            SDL_RenderCopy(renderer, textures.get(image_shells), &srcrect, &dstrect);
        }
    }
}

void BoardManager::drawInfo(SDL_Renderer * renderer) {
    // Generate texture with text
    if (this->level_updated) {
        std::string str_level = std::to_string(level);
        if (this->board->getWidth() > 7) {
            str_level = "level " + str_level;
        } else if (this->board->getWidth() > 3) {
            str_level = "L" + str_level;
        }
        if (text_level != NULL) {
            SDL_DestroyTexture(text_level);
        }
        text_level = fonts->getTexture(renderer, str_level, false, {255, 255, 255, 255});
        level_updated = false;
    }
    if (this->matches_updated) {
        std::string str_matches = std::to_string(matches);
        if (!this->isRelaxedMode) {
            str_matches += "/" +  std::to_string(required_matches);
        }
        if (text_matches != NULL) {
            SDL_DestroyTexture(text_matches);
        }
        text_matches = fonts->getTexture(renderer, str_matches, false, {255, 255, 255, 255});
        matches_updated = false;
    }
    if (this->moves_updated) {
        std::string str_moves = std::to_string(this->moves);
        if (this->board->getWidth() > 7) {
            str_moves += " moves";
        } else if (this->board->getWidth() > 3) {
            str_moves += "M";
        }
        if (text_moves != NULL) {
            SDL_DestroyTexture(text_moves);
        }
        text_moves = fonts->getTexture(renderer, str_moves, false, {255, 255, 255, 255});
        moves_updated = false;
    }

    // Render level
    if (!this->isRelaxedMode) {
        SDL_Rect rect_level = {rect_board.x, rect_board.y + rect_board.h, 0, 0};
        SDL_QueryTexture(text_level, NULL, NULL, &rect_level.w, &rect_level.h);
        rect_level.x += SHELL_SIZE / 2;
        rect_level.y += SHELL_SIZE / 2 - rect_level.h / 2;
        SDL_RenderCopy(renderer, text_level, NULL, &rect_level);
    }

    // Render matches
    this->rect_matches = {rect_board.x + rect_board.w / 2 , rect_board.y + rect_board.h, 0, 0};
    SDL_QueryTexture(text_matches, NULL, NULL, &rect_matches.w, &rect_matches.h);
    rect_matches.x -= rect_matches.w / 2;
    rect_matches.y += SHELL_SIZE / 2 - rect_matches.h / 2;
    SDL_RenderCopy(renderer, text_matches, NULL, &rect_matches);

    // Render moves
    if (!this->isRelaxedMode) {
        this->rect_moves = {rect_board.x + rect_board.w, rect_board.y + rect_board.h, 0, 0};
        SDL_QueryTexture(text_moves, NULL, NULL, &rect_moves.w, &rect_moves.h);
        rect_moves.x -= SHELL_SIZE / 2 + rect_moves.w;
        rect_moves.y += SHELL_SIZE / 2 - rect_moves.h / 2;
        SDL_RenderCopy(renderer, text_moves, NULL, &rect_moves);
    }
}
