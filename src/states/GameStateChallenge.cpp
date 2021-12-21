#include "GameStateChallenge.hpp"

#include <SDL_ttf.h>
#include <cmath>
#include <string>
#include "../colors.hpp"

GameStateChallenge::GameStateChallenge(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds) :
    theme(renderer, Theme::THEME1),
    pause_screen(renderer, "Game Paused", "Press the confirm button to exit"),
    win_screen(renderer, "Level Finished!", "Press the confirm button to continue"),
    lose_screen(renderer, "Level failed", "Press the confirm button to restart"),
    game_over_screen(renderer, "Game over", "Press the confirm button to exit")
{
    this->renderer = renderer;
    this->fonts = fonts;
    this->sounds = sounds;

    loadLevel();
}

GameStateChallenge::~GameStateChallenge() {
    SDL_DestroyTexture(text_attempts);
    SDL_DestroyTexture(text_attempts_number);
    delete(this->board);
}

void GameStateChallenge::update() {
    if (this->attempts == 0 && !this->game_over) {
        theme.pause();
        sounds->play(Sound::FAILED);
        this->game_over = true;
    } else if (this->board->isCompleted() && !this->completed  && !this->game_over) {
        theme.pause();
        sounds->play(Sound::COMPLETED);
        this->completed = true;
    } else if (!this->board->hasMovesLeft() && !this->failed && !this->game_over && !this->completed) {
        this->attempts--;
        this->attempts_changed = true;
        theme.pause();
        sounds->play(Sound::FAILED);
        this->failed = true;
    }

    if (!this->paused && !this->completed &&  !this->failed && !this->game_over) {
        this->theme.update();
        this->board->update();
    }
}

void GameStateChallenge::handleEvents(std::vector<Event> events) {
    if (!this->paused && !this->completed &&  !this->failed && !this->game_over) {
        this->board->handleEvents(events);
    }

    for(Event event: events) {
        if (this->game_over) {
            if (event == Event::CONFIRM) {
                this->done = true;
            }
        } else if (this->completed) {
            if (event == Event::CONFIRM) {
                this->completed = false;
                this->theme.next();
                this->board->loadLevel(this->position.x, this->position.y, this->width, this->height, this->moves, ++this->required_matches, ++this->level, ++this->seed);
            }
        } else if (this->failed) {
            if (event == Event::CONFIRM) {
                this->failed = false;
                this->board->reset();
                theme.unpause();
            }
        } else if (this->paused) {
            if (event == Event::CONFIRM) {
                this->done = true;
            } else if (event == Event::CANCEL || event == Event::MENU) {
                this->paused = false;
            }
        } else if (event == Event::MENU) {
            this->paused = !(this->paused);
            return;
        }
    }
}

void GameStateChallenge::draw(SDL_Renderer *renderer) {
    this->theme.draw(renderer);
    if (this->game_over) {
        game_over_screen.draw(renderer);
    } else if (this->completed) {
        win_screen.draw(renderer);
    } else if (this->failed) {
        lose_screen.draw(renderer);
    } else if (this->paused) {
        pause_screen.draw(renderer);
    } else {
        drawAttempts(renderer);
        this->board->draw(renderer);
    }
}

void GameStateChallenge::drawAttempts(SDL_Renderer * renderer) {
    if(text_attempts == NULL) {
        text_attempts = fonts->getTexture(renderer, "lives left", false, {255, 255, 255, 255});
    }
    if (this->attempts_changed) {
        if (text_attempts_number != NULL) {
            SDL_DestroyTexture(text_attempts_number);
        }
        text_attempts_number = fonts->getTexture(renderer, std::to_string(this->attempts), false, {255, 255, 255, 255});
        this->attempts_changed = false;
    }
    // Render
    SDL_Rect rect_attempts = {this->position.x, this->position.y, 0, 0};
    SDL_QueryTexture(text_attempts, NULL, NULL, &rect_attempts.w, &rect_attempts.h);
    rect_attempts.x -= rect_attempts.w + SHELL_SIZE / 2;
    SDL_RenderCopy(renderer, text_attempts, NULL, &rect_attempts);

    // Render number
    SDL_Rect rect_attempts_number = {rect_attempts.x + rect_attempts.w / 2, rect_attempts.y + rect_attempts.h, 0, 0};
    SDL_QueryTexture(text_attempts_number, NULL, NULL, &rect_attempts_number.w, &rect_attempts_number.h);
    rect_attempts_number.x -= rect_attempts_number.w / 2;
    SDL_RenderCopy(renderer, text_attempts_number, NULL, &rect_attempts_number);
}

void GameStateChallenge::loadLevel() {
    this->width = BOARD_WIDTH;
    this->height = BOARD_HEIGHT;

    this->position = calculatePosition(this->width, this->height);

    this->moves = 10;
    this->required_matches = 12;
    this->level = 1;
    this->seed = this->level;

    this->board = new BoardManager(
        renderer,
        this->fonts,
        this->sounds,
        this->position.x,
        this->position.y,
        this->width,
        this->height,
        this->moves,
        this->required_matches,
        this->level,
        this->seed
    );

    this->attempts = 3;
    this->attempts_changed = true;
}

SDL_Point GameStateChallenge::calculatePosition(int width, int height) {
    return {(SCREEN_WIDTH-SHELL_SIZE*width)/2, (SCREEN_HEIGHT-SHELL_SIZE*(height+1))/2};
}

State GameStateChallenge::getNextState() {
    return State::MENU;
}
