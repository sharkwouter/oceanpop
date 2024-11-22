#include "BoardManager.hpp"

#include <iostream>
#include <string>
#include <cmath>

#include "Sound.hpp"
#include "colors.hpp"
#include "FontType.hpp"
#include "utils.hpp"

BoardManager::BoardManager(SDL_Renderer *renderer, FontManager *fonts, SoundManager * sounds, OptionManager * options, int width, int height, int moves, int required_matches, int level, int seed) : fonts(fonts), sounds(sounds), options(options),
    textures(renderer, options) {
    this->previous_shell_size = this->options->getShellSize();
    loadLevel(width, height, moves, required_matches, level, seed);
}

BoardManager::BoardManager(SDL_Renderer *renderer, FontManager *fonts, SoundManager * sounds, OptionManager * options, std::vector<std::vector<ShellType>> shells, int moves, int required_matches, int level, int seed) : fonts(fonts), sounds(sounds), options(options),
    textures(renderer, options) {
    this->previous_shell_size = this->options->getShellSize();
    loadLevel(shells, moves, required_matches, level, seed);
}

BoardManager::~BoardManager() {
    delete(this->board);
    SDL_DestroyTexture(text_level);
    SDL_DestroyTexture(text_matches);
    SDL_DestroyTexture(text_moves);
    SDL_DestroyTexture(text_plus_one);
    SDL_DestroyTexture(text_plus_two);
    SDL_DestroyTexture(text_plus_three);
    SDL_DestroyTexture(text_plus_six);
    SDL_DestroyTexture(text_minus_three);
    SDL_DestroyTexture(text_minus_six);
}

void BoardManager::loadLevel(int width, int height, int moves, int required_matches, int level, int seed) {
    this->seed = seed;
    if (required_matches == 0) {
        this->isRelaxedMode = true;
    }
    storeLevel(new Board(width, height, seed, this->isRelaxedMode), moves, required_matches, level);
    this->preview = this->board->getShells();
    this->current_action = Action::INTRODUCTION_START;
}

void BoardManager::loadLevel(std::vector<std::vector<ShellType>> shells, int moves, int required_matches, int level, int seed) {
    this->starting_shells = shells;
    this->seed = seed;
    if (required_matches == 0) {
        this->isRelaxedMode = true;
    }
    storeLevel(new Board(shells, seed, this->isRelaxedMode), moves, required_matches, level);
    this->preview = this->board->getShells();
    this->current_action = Action::INTRODUCTION_START;
}

void BoardManager::storeLevel(Board * board, int moves, int required_matches, int level) {
    if (this->board != NULL) {
        delete(this->board);
    }
    this->board = std::move(board);

    this->setSizing();

    this->selected.x = this->board->getWidth() / 2;
    this->selected.y = this->board->getHeight() / 2;

    this->matches = 0;
    this->required_matches = required_matches;

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
                } else if (this->current_action == Action::ANIMATE_INTRODUCTION) {
                    this->current_action = Action::PICKING;
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
            case Event::WINDOW_RESIZE:
                this->setSizing();
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
        SDL_Point newSelected = {(mouse.x - rect_board.x)/this->options->getShellSize(), (mouse.y - rect_board.y)/this->options->getShellSize()};
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

void BoardManager::increaseLevel() {
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
                increaseLevel();
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
            if(delta >= (DROP_TIME/this->options->getShellSize())) {
                this->animation += delta/(DROP_TIME/this->options->getShellSize());
                this->animation_start = SDL_GetTicks();
            }
            if (this->animation > this->options->getShellSize()) {
                this->current_action = Action::FALLING_START;
                this->preview = this->board->getShells();
            }
            break;
        case Action::FALLING_END:
            this->preview = this->board->getShells();
            this->current_action = Action::MATCHING_START;
            break;
        case Action::INTRODUCTION_START:
            this->preview = this->board->getShells();
            this->animation = 0;
            this->animation_start = SDL_GetTicks();
            this->current_action = Action::ANIMATE_INTRODUCTION;
            break;
        case Action::ANIMATE_INTRODUCTION:
            delta = SDL_GetTicks() - this->animation_start;
            if(delta >= (DROP_TIME/this->options->getShellSize())) {
                this->animation += delta/(DROP_TIME/this->options->getShellSize());
                this->animation_start = SDL_GetTicks();
            }
            if (this->animation > this->board->getHeight() * this->options->getShellSize()) {
                this->animation = 0;
                this->animation_start = SDL_GetTicks();
                this->current_action = Action::INTRODUCTION_END;
            }
            break;
        case Action::INTRODUCTION_END:
            delta = SDL_GetTicks() - this->animation_start;
            if(delta >= (DROP_TIME/this->options->getShellSize())) {
                this->animation += delta/(DROP_TIME/this->options->getShellSize());
                this->animation_start = SDL_GetTicks();
            }
            if (this->animation > this->options->getShellSize()) {
                this->current_action = Action::PICKING;
            }
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
    drawScoringBox(renderer);
    if (this->current_action > Action::INTRODUCTION_END) {
        drawCursor(renderer);
        drawShells(renderer);
        drawInfo(renderer);
    } else {
        drawIntroduction(renderer);
    }

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
        pickrect_hor.y = this->options->getShellSize() * this->picked.y + this->rect_board.y + 1;
        pickrect_hor.w = this->options->getShellSize() * this->board->getWidth() - 1;
        pickrect_hor.h = this->options->getShellSize() - 1;

        SDL_Rect pickrect_ver;
        pickrect_ver.x = this->options->getShellSize() * this->picked.x + this->rect_board.x + 1;
        pickrect_ver.y = this->rect_board.y + 1;
        pickrect_ver.w = this->options->getShellSize() - 1;
        pickrect_ver.h =  this->options->getShellSize() * this->board->getHeight() - 1;

        SDL_SetRenderDrawColor(renderer, COLOR_PICKED.r, COLOR_PICKED.g, COLOR_PICKED.b, COLOR_PICKED.a);
        SDL_RenderFillRect(renderer, &pickrect_ver);
        SDL_RenderFillRect(renderer, &pickrect_hor);
    }

    // Draw selection rectangle
    SDL_Rect selrect;
    selrect.x = this->options->getShellSize() * selected.x + this->rect_board.x + 1;
    selrect.y = this->options->getShellSize() * selected.y + this->rect_board.y + 1;
    selrect.w = this->options->getShellSize() - 1;
    selrect.h = this->options->getShellSize() - 1;

    SDL_SetRenderDrawColor(renderer, COLOR_SELECT.r, COLOR_SELECT.g, COLOR_SELECT.b, COLOR_SELECT.a);
    SDL_RenderFillRect(renderer, &selrect);
}

void BoardManager::drawBoard(SDL_Renderer * renderer) {
    // Draw background rectangle
    SDL_SetRenderDrawColor(renderer, COLOR_BOARD.r, COLOR_BOARD.g, COLOR_BOARD.b, COLOR_BOARD.a);
    SDL_RenderFillRect(renderer, &this->rect_board);

    // Draw board lines
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int x = 0; x <= this->board->getWidth(); x++) {
        int current_x = this->rect_board.x + x * this->options->getShellSize();
        SDL_RenderDrawLine(
                renderer,
                current_x,
                this->rect_board.y,
                current_x,
                (this->rect_board.y + this->rect_board.h)
                );
    }
    for (int y = 0; y <= this->board->getHeight(); y++) {
        int current_y = this->rect_board.y + y * this->options->getShellSize();
        SDL_RenderDrawLine(
                renderer,
                this->rect_board.x,
                current_y,
                (this->rect_board.x + this->rect_board.w),
                current_y
        );
    }
}

void BoardManager::drawShells(SDL_Renderer * renderer) {
    // Draw the shells
    for (int x = 0; x < this->board->getWidth(); x++) {
        for (int y = 0; y < this->board->getHeight(); y++) {
            if (this->current_action == Action::ANIMATE_FALLING && isFalling({x,y})) {
                continue;
            }

            SDL_Rect srcrect;
            srcrect.x = this->options->getShellSize() * (int) this->preview[x][y];
            srcrect.y = 0;
            srcrect.w = this->options->getShellSize();
            srcrect.h = this->options->getShellSize();

            SDL_Rect dstrect;
            dstrect.x = this->options->getShellSize() * x + this->rect_board.x;
            dstrect.y = this->options->getShellSize() * y + this->rect_board.y;
            dstrect.w = this->options->getShellSize();
            dstrect.h = this->options->getShellSize();

            SDL_RenderCopy(renderer, textures.getShellTexture(), &srcrect, &dstrect);
        }
    }
}

void BoardManager::drawFallingShells(SDL_Renderer * renderer) {   
    // Draw the shells
    for(Shell shell : this->shells_to_drop) {
        SDL_Rect srcrect;
        srcrect.x = this->options->getShellSize() * (int) shell.type;
        srcrect.y = 0;
        srcrect.w = this->options->getShellSize();
        srcrect.h = this->options->getShellSize();

        SDL_Rect dstrect;
        dstrect.x = this->options->getShellSize() * shell.x + this->rect_board.x;
        dstrect.y = this->options->getShellSize() * shell.y + this->rect_board.y + animation;
        dstrect.w = this->options->getShellSize();
        dstrect.h = this->options->getShellSize();

        if (shell.y == -1) {
            int start_y = this->rect_board.y - dstrect.y + 1;
            dstrect.y = this->rect_board.y;
            dstrect.h = this->options->getShellSize() - start_y -1;
            srcrect.y = start_y;
            srcrect.h = dstrect.h;
        }

        SDL_RenderCopy(renderer, textures.getShellTexture(), &srcrect, &dstrect);
    }
}

void BoardManager::drawIntroduction(SDL_Renderer * renderer) {
    // Draw the shells
    float current_deduction = this->options->getShellSize() * this->board->getHeight() - animation;
    if (this->current_action == Action::INTRODUCTION_END) {
        current_deduction = 0.0f;
    }
    for (int x = 0; x < this->board->getWidth(); x++) {
        for (int y = 0; y < this->board->getHeight(); y++) {
            if (this->options->getShellSize() * (y + 1) < current_deduction) {
                continue;
            }

            SDL_Rect srcrect;
            srcrect.x = this->options->getShellSize() * (int) this->preview[x][y];
            srcrect.y = 0;
            srcrect.w = this->options->getShellSize();
            srcrect.h = this->options->getShellSize();

            SDL_Rect dstrect;
            dstrect.x = this->options->getShellSize() * x + this->rect_board.x;
            dstrect.y = this->options->getShellSize() * y + this->rect_board.y - current_deduction;
            dstrect.w = this->options->getShellSize();
            dstrect.h = this->options->getShellSize();

            if (dstrect.y < this->rect_board.y) {
                int start_y = this->rect_board.y - dstrect.y + 1;
                dstrect.y = this->rect_board.y;
                dstrect.h = this->options->getShellSize() - start_y - 1;
                srcrect.y = start_y;
                srcrect.h = dstrect.h;
            }

            SDL_RenderCopy(renderer, textures.getShellTexture(), &srcrect, &dstrect);
        }
    }
    if (this->level_updated) {
        SDL_DestroyTexture(this->text_introduction);
        this->text_introduction = NULL;
    }
    if (this->text_introduction == NULL) {
        std::string introduction_string;
        if (this->isRelaxedMode) {
            introduction_string = _("Relax there is no goal");
        } else {
            introduction_string = _("Score ") + std::to_string(required_matches) + _(" points in ") + std::to_string(moves) + _(" moves");
        }
        this->text_introduction = fonts->getTexture(renderer, introduction_string, FontType::NORMAL, {255, 255, 255, 255});
    }
    // Render Introduction text
    SDL_Rect rect_introduction = {rect_scoreboard.x + rect_scoreboard.w / 2 , rect_scoreboard.y, 0, 0};
    SDL_QueryTexture(text_introduction, NULL, NULL, &rect_introduction.w, &rect_introduction.h);
    rect_introduction.x -= rect_introduction.w / 2;
    rect_introduction.y += this->options->getShellSize() / 2 - rect_introduction.h / 2;

    if (this->current_action == Action::INTRODUCTION_END) {
        rect_introduction.y += animation;
    }

    SDL_RenderCopy(renderer, text_introduction, NULL, &rect_introduction);
}

bool BoardManager::isDoubleMatch(Match match) {
    for(Match m : this->matches_made) {
        if (m.direction != match.direction) {
            if (m.direction == Direction::HORIZONTAL) {
                if (m.x+1 == match.x && m.y == match.y+1) {
                    return true;
                }
            } else {
                if (m.x == match.x+1 && m.y+1 == match.y) {
                    return true;
                }
            }
        }
    }
    return false;
}

void BoardManager::drawMatches(SDL_Renderer * renderer) {
    if (this->text_plus_one == NULL) {
        this->text_plus_one = fonts->getTexture(renderer, "+1", FontType::NORMAL, {255, 255, 255, 255});
    }
    if (this->text_plus_two == NULL) {
        this->text_plus_two = fonts->getTexture(renderer, "+2", FontType::NORMAL, {255, 255, 255, 255});
    }
    if (!this->isRelaxedMode && this->text_minus_three == NULL) {
        this->text_minus_three = fonts->getTexture(renderer, "-3", FontType::NORMAL, {0, 0, 0, 255});
    }
    if (!this->isRelaxedMode && this->text_minus_six == NULL) {
        this->text_minus_six = fonts->getTexture(renderer, "-6", FontType::NORMAL, {0, 0, 0, 255});
    }
    if (this->isRelaxedMode && this->text_plus_three == NULL) {
        this->text_plus_three = fonts->getTexture(renderer, "+3", FontType::NORMAL, {255, 255, 255, 255});
    }
    if (this->isRelaxedMode && this->text_plus_six == NULL) {
        this->text_plus_six = fonts->getTexture(renderer, "+6", FontType::NORMAL, {255, 255, 255, 255});
    }

    for(Match match : this->matches_made) {
        // Draw the shells
        int current_shell_size = this->options->getShellSize();
        if (match.type != ShellType::BUBBLE || this->isRelaxedMode) {
            current_shell_size = (this->options->getShellSize()*(MATCH_STEPS-animation))/MATCH_STEPS;
        }
        for(int i = 0; i < 3; i++) {
            SDL_Rect srcrect;
            srcrect.x = this->options->getShellSize() * (int) match.type;
            srcrect.y = 0;
            srcrect.w = this->options->getShellSize();
            srcrect.h = this->options->getShellSize();

            SDL_Rect dstrect;
            dstrect.x = this->options->getShellSize() * match.x + this->rect_board.x + (this->options->getShellSize() - current_shell_size) / 2;
            dstrect.y = this->options->getShellSize() * match.y + this->rect_board.y + (this->options->getShellSize() - current_shell_size) / 2;
            dstrect.w = current_shell_size;
            dstrect.h = current_shell_size;

            if (match.direction == Direction::HORIZONTAL) {
                dstrect.x += this->options->getShellSize()*i;
            } else {
                dstrect.y += this->options->getShellSize()*i;
            }

            // Move towards score/moves
            if (match.type == ShellType::BUBBLE && !this->isRelaxedMode) {
                dstrect.x += (rect_moves.x-dstrect.x)/MATCH_STEPS*animation;
                dstrect.y += (rect_moves.y-dstrect.y)/MATCH_STEPS*animation;
            }

            SDL_RenderCopy(renderer, textures.getShellTexture(), &srcrect, &dstrect);
        }

        if (this->isRelaxedMode || match.type != ShellType::BUBBLE) {
            // Draw the score text
            SDL_Texture * text = text_plus_one;
            if (this->isDoubleMatch(match)) {
                text = text_plus_two;
            }
            if (match.type == ShellType::URCHIN) {
                if (text == text_plus_two) {
                    text = text_minus_six;
                } else {
                    text = text_minus_three;
                }
            }
            if (match.type == ShellType::BUBBLE) {
                if (text == text_plus_two) {
                    text = text_plus_six;
                } else {
                    text = text_plus_three;
                }
            }

            SDL_Rect rect_plus_one = {this->options->getShellSize() * match.x + this->rect_board.x, this->options->getShellSize() * match.y + this->rect_board.y, 0, 0};
            SDL_QueryTexture(text, NULL, NULL, &rect_plus_one.w, &rect_plus_one.h);
            rect_plus_one.x += this->options->getShellSize()/2 - rect_plus_one.w/2;
            rect_plus_one.y += this->options->getShellSize()/2 - rect_plus_one.h/2;
            if (match.direction == Direction::HORIZONTAL) {
                rect_plus_one.x += this->options->getShellSize();
            } else {
                rect_plus_one.y += this->options->getShellSize();
            }

            // Move towards score
            rect_plus_one.x += (rect_matches.x-rect_plus_one.x)/MATCH_STEPS*animation;
            rect_plus_one.y += (rect_matches.y-rect_plus_one.y)/MATCH_STEPS*animation;

            SDL_RenderCopy(renderer, text, NULL, &rect_plus_one);
        }
    }
}

void BoardManager::drawScoringBox(SDL_Renderer * renderer) {
    // Draw score board box
    SDL_SetRenderDrawColor(renderer, COLOR_BOARD.r, COLOR_BOARD.g, COLOR_BOARD.b, COLOR_BOARD.a);
    SDL_RenderFillRect(renderer, &rect_scoreboard);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(renderer,
        rect_scoreboard.x,
        rect_scoreboard.y,
        rect_scoreboard.x +rect_scoreboard.w,
        rect_scoreboard.y
    );
}

void BoardManager::drawInfo(SDL_Renderer * renderer) {
    // Generate texture with text
    if (this->level_updated) {
        std::string str_level = std::to_string(level);
        str_level = _("level ") + str_level;
        if (text_level != NULL) {
            SDL_DestroyTexture(text_level);
        }
        text_level = fonts->getTexture(renderer, str_level, FontType::NORMAL, {255, 255, 255, 255});
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
        text_matches = fonts->getTexture(renderer, str_matches, FontType::NORMAL, {255, 255, 255, 255});
        matches_updated = false;
    }
    if (this->moves_updated) {
        std::string str_moves = std::to_string(this->moves);
        str_moves += _(" moves");
        if (text_moves != NULL) {
            SDL_DestroyTexture(text_moves);
        }
        text_moves = fonts->getTexture(renderer, str_moves, FontType::NORMAL, {255, 255, 255, 255});
        moves_updated = false;
    }

    // Render level
    if (!this->isRelaxedMode) {
        SDL_Rect rect_level = {rect_scoreboard.x, rect_scoreboard.y, 0, 0};
        SDL_QueryTexture(text_level, NULL, NULL, &rect_level.w, &rect_level.h);
        rect_level.x += this->options->getShellSize() / 2;
        rect_level.y += this->options->getShellSize() / 2 - rect_level.h / 2;
        SDL_RenderCopy(renderer, text_level, NULL, &rect_level);
    }

    // Render matches
    this->rect_matches = {rect_scoreboard.x + rect_scoreboard.w / 2 , rect_scoreboard.y, 0, 0};
    SDL_QueryTexture(text_matches, NULL, NULL, &rect_matches.w, &rect_matches.h);
    rect_matches.x -= rect_matches.w / 2;
    rect_matches.y += this->options->getShellSize() / 2 - rect_matches.h / 2;
    SDL_RenderCopy(renderer, text_matches, NULL, &rect_matches);

    // Render moves
    if (!this->isRelaxedMode) {
        this->rect_moves = {rect_scoreboard.x + rect_scoreboard.w, rect_scoreboard.y, 0, 0};
        SDL_QueryTexture(text_moves, NULL, NULL, &rect_moves.w, &rect_moves.h);
        rect_moves.x -= this->options->getShellSize() / 2 + rect_moves.w;
        rect_moves.y += this->options->getShellSize() / 2 - rect_moves.h / 2;
        SDL_RenderCopy(renderer, text_moves, NULL, &rect_moves);
    }
}

void BoardManager::setSizing() {
    // Set the board location and size
    this->rect_board.w = this->options->getShellSize() * this->board->getWidth();
    this->rect_board.h = this->options->getShellSize() * this->board->getHeight();
    this->rect_board.x = (this->options->getScreenWidth() - (this->options->getShellSize()*this->board->getWidth()))/2;
    this->rect_board.y = (this->options->getScreenHeight() - this->options->getShellSize()*(this->board->getHeight()+1))/2;

    // Set the scoreboard location and size
    this->rect_scoreboard.x = 0;
    this->rect_scoreboard.y = this->options->getScreenHeight() - this->options->getShellSize();
    this->rect_scoreboard.w = this->options->getScreenWidth();
    this->rect_scoreboard.h = this->options->getShellSize();

    // Do not reload textures if the shell size did not change
    if (this->previous_shell_size == this->options->getShellSize()) {
        return;
    }

    // Reset texts
    this->matches_updated = true;
    this->level_updated = true;
    this->moves_updated = true;

    // Free textures if they are already set
    if (this->text_introduction) {
        SDL_DestroyTexture(this->text_introduction);
        this->text_introduction = NULL;
    }
    if (this->text_plus_one) {
        SDL_DestroyTexture(this->text_plus_one);
        this->text_plus_one = NULL;
    }
    if (this->text_plus_two) {
        SDL_DestroyTexture(this->text_plus_two);
        this->text_plus_two = NULL;
    }
    if (this->text_minus_three) {
        SDL_DestroyTexture(this->text_minus_three);
        this->text_minus_three = NULL;
    }
    if (this->text_minus_six) {
        SDL_DestroyTexture(this->text_minus_six);
        this->text_minus_six = NULL;
    }
    if (this->text_plus_three) {
        SDL_DestroyTexture(this->text_plus_three);
        this->text_plus_three = NULL;
    }
    if (this->text_plus_six) {
        SDL_DestroyTexture(this->text_plus_six);
        this->text_plus_six = NULL;
    }

    // Load a new fitting shell texture
    this->textures.reload();

    // Update the shell size
    previous_shell_size = this->options->getShellSize();
}
