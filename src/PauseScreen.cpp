#include "PauseScreen.hpp"

#include "constants.hpp"
#include "colors.hpp"

PauseScreen::PauseScreen(SDL_Renderer * renderer, std::string title, std::string subtitle) {
        this->title = fonts.getTexture(renderer, title, true, {255, 255, 255, 255});
        this->subtitle = fonts.getTexture(renderer, subtitle, false, {255, 255, 255, 255});
}

PauseScreen::~PauseScreen() {
    SDL_DestroyTexture(title);
    SDL_DestroyTexture(subtitle);
}

void PauseScreen::draw(SDL_Renderer * renderer) {
    SDL_Rect rect_completed = {SCREEN_WIDTH/2, SCREEN_HEIGHT/3, 0, 0};
    SDL_QueryTexture(title, NULL, NULL, &rect_completed.w, &rect_completed.h);
    rect_completed.x -=  rect_completed.w/2;
    rect_completed.y -= rect_completed.h/2;

    SDL_Rect rect_completed_subtitle = {SCREEN_WIDTH/2, rect_completed.y + rect_completed.h, 0, 0};
    SDL_QueryTexture(subtitle, NULL, NULL, &rect_completed_subtitle.w, &rect_completed_subtitle.h);
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

    SDL_RenderCopy(renderer, title, NULL, &rect_completed);
    SDL_RenderCopy(renderer, subtitle, NULL, &rect_completed_subtitle);
}