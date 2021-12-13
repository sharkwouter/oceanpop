#include "GameState.hpp"

#include <SDL_ttf.h>
#include <cmath>
#include "../colors.hpp"

GameState::GameState(SDL_Renderer *renderer) :
    board(renderer, &fonts, (SCREEN_WIDTH-SHELL_SIZE*BOARD_WIDTH)/2, (SCREEN_HEIGHT-SHELL_SIZE*(BOARD_HEIGHT+1))/2, BOARD_WIDTH, BOARD_HEIGHT),
    theme(renderer, Theme::THEME1),
    pause_screen(renderer, "Game Paused", "Press the confirm button to exit"),
    win_screen(renderer, "Level Finished!", "Press the confirm button to continue"),
    lose_screen(renderer, "Level failed", "Press the confirm button to restart")
{

}

GameState::~GameState() {

}

void GameState::update() {
    this->theme.update();
    if (!this->paused && !this->board.isCompleted() && this->board.getMoves() > 0) {
        this->board.update();
    }
}

void GameState::handleEvents(std::vector<Event> events) {
    if (!this->paused) {
        this->board.handleEvents(events);
    }

    for(Event event: events) {
        if (event == Event::MENU) {
            this->paused = !(this->paused);
            return;
        }

        if (this->paused) {
            if (event == Event::CONFIRM) {
                this->done = true;
            } else if (event == Event::CANCEL) {
                this->paused = false;
            }
        } else if (this->board.isCompleted()) {
            if (event == Event::CONFIRM) {
                this->theme.next();
                this->board.reset();
            }
        } else if (this->board.getMoves() == 0) {
            if (event == Event::CONFIRM) {
                this->board.reset();
            }
        }
    }
}

void GameState::draw(SDL_Renderer *renderer) {
    this->theme.draw(renderer);
    if (this->board.isCompleted()) {
        win_screen.draw(renderer);
    } else if (this->board.getMoves() == 0) {
        lose_screen.draw(renderer);
    } else if (this->paused) {
        pause_screen.draw(renderer);
    } else {
        this->board.draw(renderer);
    }
}

State GameState::getNextState() {
    return State::MENU;
}
