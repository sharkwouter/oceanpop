#include "GameState.hpp"

#include <SDL_ttf.h>
#include <cmath>
#include "../colors.hpp"

GameState::GameState(SDL_Renderer * renderer) :
    theme(renderer, Theme::THEME1),
    pause_screen(renderer, "Game Paused", "Press the confirm button to exit"),
    win_screen(renderer, "Level Finished!", "Press the confirm button to continue"),
    lose_screen(renderer, "Level failed", "Press the confirm button to restart")
{
    this->renderer = renderer;
    loadLevel();
}

GameState::~GameState() {
    delete(this->board);
}

void GameState::update() {
    this->theme.update();
    if (!this->paused && !this->board->isCompleted() && this->board->hasMovesLeft() > 0) {
        this->board->update();
    }
}

void GameState::handleEvents(std::vector<Event> events) {
    if (!this->paused) {
        this->board->handleEvents(events);
    }

    for(Event event: events) {
        if (event == Event::MENU) {
            //this->paused = !(this->paused);
            this->board->reset();
            return;
        }

        if (this->paused) {
            if (event == Event::CONFIRM) {
                this->done = true;
            } else if (event == Event::CANCEL) {
                this->paused = false;
            }
        } else if (this->board->isCompleted()) {
            if (event == Event::CONFIRM) {
                this->theme.next();
                this->board->loadLevel(this->position.x, this->position.y, this->width, this->height, this->moves, ++this->required_matches, ++this->level);
            }
        } else if (!this->board->hasMovesLeft()) {
            if (event == Event::CONFIRM) {
                this->board->reset();
            }
        }
    }
}

void GameState::draw(SDL_Renderer *renderer) {
    this->theme.draw(renderer);
    if (this->board->isCompleted()) {
        win_screen.draw(renderer);
    } else if (!this->board->hasMovesLeft()) {
        lose_screen.draw(renderer);
    } else if (this->paused) {
        pause_screen.draw(renderer);
    } else {
        this->board->draw(renderer);
    }
}

void GameState::loadLevel() {
    this->width = BOARD_WIDTH;
    this->height = BOARD_HEIGHT;

    this->position = calculatePosition(this->width, this->height);

    this->moves = 10;
    this->required_matches = 12;
    this->level = 1;

    this->board = new BoardManager(
        renderer,
        &this->fonts,
        this->position.x,
        this->position.y,
        this->width,
        this->height,
        this->moves,
        this->required_matches,
        this->level
    );
}

SDL_Point GameState::calculatePosition(int width, int height) {
    return {(SCREEN_WIDTH-SHELL_SIZE*width)/2, (SCREEN_HEIGHT-SHELL_SIZE*(height+1))/2};
}

State GameState::getNextState() {
    return State::MENU;
}
