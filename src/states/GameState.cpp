#include "GameState.hpp"

#include <SDL_ttf.h>
#include <cmath>
#include "../colors.hpp"

GameState::GameState(SDL_Renderer *renderer) :
    board(renderer, &fonts, (SCREEN_WIDTH-SHELL_SIZE*BOARD_WIDTH)/2, (SCREEN_HEIGHT-SHELL_SIZE*(BOARD_HEIGHT+1))/2, BOARD_WIDTH, BOARD_HEIGHT),
    theme(renderer, 1, true)
{
    this->text_paused = fonts.getTexture(renderer, "Game Paused", true, {255, 255, 255, 255});
    this->text_paused_subtitle = fonts.getTexture(renderer, "Press the confirm button to exit", false, {255, 255, 255, 255});
    this->running = true;
}

GameState::~GameState() {

}

void GameState::update() {
    this->theme.update();
    if (!this->paused) {
        this->board.update();
    }
}

void GameState::handleEvents(std::vector<Event> events) {
    for(Event event: events) {
        if (event == Event::MENU) {
                this->paused = !(this->paused);
                return;
        } else if (event != Event::NONE && event != Event::MOUSEMOVE && this->paused) {
            if (event == Event::CONFIRM) {
                this->running = false;
            } else {
                this->paused = false;
            }
        }
    }
    if (!this->paused) {
        this->board.handleEvents(events);
    }
}

void GameState::draw(SDL_Renderer *renderer) {
    this->theme.draw(renderer);
    if (!this->paused) {
        this->board.draw(renderer);
    } else {
        drawPauseScreen(renderer);
    }
}

void GameState::drawPauseScreen(SDL_Renderer * renderer) {
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