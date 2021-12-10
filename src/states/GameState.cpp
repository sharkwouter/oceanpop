#include "GameState.hpp"

#include <SDL_ttf.h>
#include <cmath>
#include "../colors.hpp"

GameState::GameState(SDL_Renderer *renderer) :
    board(renderer, &fonts, (SCREEN_WIDTH-SHELL_SIZE*BOARD_WIDTH)/2, (SCREEN_HEIGHT-SHELL_SIZE*(BOARD_HEIGHT+1))/2, BOARD_WIDTH, BOARD_HEIGHT),
    theme(renderer, Theme::DEFAULT)
{

}

GameState::~GameState() {
    SDL_DestroyTexture(text_paused);
    SDL_DestroyTexture(text_paused_subtitle);
}

void GameState::update() {
    this->theme.update();
    if (!this->paused && !this->board.isCompleted()) {
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
                this->board.reset();
            }
        }
    }
}

void GameState::draw(SDL_Renderer *renderer) {
    this->theme.draw(renderer);
    if (this->board.isCompleted()) {
        drawCompletedScreen(renderer);
    } else if (this->paused) {
        drawPauseScreen(renderer);
    } else {
        this->board.draw(renderer);
    }
}

void GameState::drawPauseScreen(SDL_Renderer * renderer) {
    if (this->text_paused == NULL) {
        this->text_paused = fonts.getTexture(renderer, "Game Paused", true, {255, 255, 255, 255});
    }
    if (this->text_paused_subtitle == NULL) {
        this->text_paused_subtitle = fonts.getTexture(renderer, "Press the confirm button to exit", false, {255, 255, 255, 255});
    }

    SDL_Rect rect_paused = {SCREEN_WIDTH/2, SCREEN_HEIGHT/3, 0, 0};
    SDL_QueryTexture(text_paused, NULL, NULL, &rect_paused.w, &rect_paused.h);
    rect_paused.x -=  rect_paused.w/2;
    rect_paused.y -= rect_paused.h/2;

    SDL_Rect rect_paused_subtitle = {SCREEN_WIDTH/2, rect_paused.y + rect_paused.h, 0, 0};
    SDL_QueryTexture(text_paused_subtitle, NULL, NULL, &rect_paused_subtitle.w, &rect_paused_subtitle.h);
    rect_paused_subtitle.x -=  rect_paused_subtitle.w/2;

    SDL_Rect rect_background;
    rect_background.x = std::min(rect_paused.x, rect_paused_subtitle.x) - SHELL_SIZE/2;
    rect_background.y = std::min(rect_paused.y, rect_paused_subtitle.y) - SHELL_SIZE/2;
    rect_background.w = std::max(rect_paused.w, rect_paused_subtitle.w) + SHELL_SIZE;
    rect_background.h = rect_paused_subtitle.y - rect_paused.y + rect_paused_subtitle.h + SHELL_SIZE;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect_background);

    SDL_SetRenderDrawColor(renderer, COLOR_BOARD.r, COLOR_BOARD.g, COLOR_BOARD.b, COLOR_BOARD.a);
    SDL_RenderFillRect(renderer, &rect_background);

    SDL_RenderCopy(renderer, text_paused, NULL, &rect_paused);
    SDL_RenderCopy(renderer, text_paused_subtitle, NULL, &rect_paused_subtitle);
}

void GameState::drawCompletedScreen(SDL_Renderer * renderer) {
    if (this->text_completed == NULL) {
        this->text_completed = fonts.getTexture(renderer, "Level Finished!", true, {255, 255, 255, 255});
    }
    if (this->text_completed_subtitle == NULL) {
        this->text_completed_subtitle = fonts.getTexture(renderer, "Press the confirm button to continue", false, {255, 255, 255, 255});
    }

    SDL_Rect rect_completed = {SCREEN_WIDTH/2, SCREEN_HEIGHT/3, 0, 0};
    SDL_QueryTexture(text_completed, NULL, NULL, &rect_completed.w, &rect_completed.h);
    rect_completed.x -=  rect_completed.w/2;
    rect_completed.y -= rect_completed.h/2;

    SDL_Rect rect_completed_subtitle = {SCREEN_WIDTH/2, rect_completed.y + rect_completed.h, 0, 0};
    SDL_QueryTexture(text_completed_subtitle, NULL, NULL, &rect_completed_subtitle.w, &rect_completed_subtitle.h);
    rect_completed_subtitle.x -=  rect_completed_subtitle.w/2;

    SDL_Rect rect_background;
    rect_background.x = std::min(rect_completed.x, rect_completed_subtitle.x) - SHELL_SIZE/2;
    rect_background.y = std::min(rect_completed.y, rect_completed_subtitle.y) - SHELL_SIZE/2;
    rect_background.w = std::max(rect_completed.w, rect_completed_subtitle.w) + SHELL_SIZE;
    rect_background.h = rect_completed_subtitle.y - rect_completed.y + rect_completed_subtitle.h + SHELL_SIZE;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect_background);

    SDL_SetRenderDrawColor(renderer, COLOR_BOARD.r, COLOR_BOARD.g, COLOR_BOARD.b, COLOR_BOARD.a);
    SDL_RenderFillRect(renderer, &rect_background);

    SDL_RenderCopy(renderer, text_completed, NULL, &rect_completed);
    SDL_RenderCopy(renderer, text_completed_subtitle, NULL, &rect_completed_subtitle);
}

State GameState::getNextState() {
    return State::MENU;
}
