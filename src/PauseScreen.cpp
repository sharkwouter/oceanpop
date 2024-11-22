#include "PauseScreen.hpp"

#include "constants.hpp"
#include "colors.hpp"
#include "FontType.hpp"

PauseScreen::PauseScreen(SDL_Renderer * renderer, FontManager * fonts, OptionManager * options, std::string title, std::string subtitle) : fonts(fonts), options(options), title_string(title), subtitle_string(subtitle) {
}

PauseScreen::~PauseScreen() {
    SDL_DestroyTexture(title);
    SDL_DestroyTexture(subtitle);
}

void PauseScreen::draw(SDL_Renderer * renderer) {
    if (!this->title) {
        this->title = fonts->getTexture(renderer, this->title_string, FontType::TITLE, {255, 255, 255, 255});
    }
    SDL_Rect rect_completed = {this->options->getScreenWidth()/2, this->options->getScreenHeight()/3, 0, 0};
    SDL_QueryTexture(title, NULL, NULL, &rect_completed.w, &rect_completed.h);
    rect_completed.x -=  rect_completed.w/2;
    rect_completed.y -= rect_completed.h/2;

    if (!this->subtitle) {
        this->subtitle = fonts->getTexture(renderer, this->subtitle_string, FontType::NORMAL, {255, 255, 255, 255});
    }
    SDL_Rect rect_completed_subtitle = {this->options->getScreenWidth()/2, rect_completed.y + rect_completed.h, 0, 0};
    SDL_QueryTexture(subtitle, NULL, NULL, &rect_completed_subtitle.w, &rect_completed_subtitle.h);
    rect_completed_subtitle.x -=  rect_completed_subtitle.w/2;

    SDL_Rect rect_background;
    rect_background.x = std::min(rect_completed.x, rect_completed_subtitle.x) - this->options->getShellSize()/2;
    rect_background.y = std::min(rect_completed.y, rect_completed_subtitle.y) - this->options->getShellSize()/2;
    rect_background.w = std::max(rect_completed.w, rect_completed_subtitle.w) + this->options->getShellSize();
    rect_background.h = rect_completed_subtitle.y - rect_completed.y + rect_completed_subtitle.h + this->options->getShellSize();

    SDL_SetRenderDrawColor(renderer, COLOR_BOARD.r, COLOR_BOARD.g, COLOR_BOARD.b, COLOR_BOARD.a);
    SDL_RenderFillRect(renderer, &rect_background);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect_background);

    SDL_RenderCopy(renderer, title, NULL, &rect_completed);
    SDL_RenderCopy(renderer, subtitle, NULL, &rect_completed_subtitle);
}

void PauseScreen::updateSizing() {
    if (this->title) {
        SDL_DestroyTexture(this->title);
        this->title = NULL;
    }
    if (this->subtitle) {
        SDL_DestroyTexture(this->subtitle);
        this->subtitle = NULL;
    }
}