#include "CreditsState.hpp"

#include "../constants.hpp"
#include "../colors.hpp"
#include "../FontType.hpp"
#include "../utils.hpp"
#include "GameState.hpp"

#include <fstream>
#include <iostream>

CreditsState::CreditsState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options) : renderer(renderer), fonts(fonts), sounds(sounds), options(options),
    theme(renderer, options, Theme::MENU)
{
    this->text_title = fonts->getTexture(renderer, _("Credits"), FontType::TITLE, {COLOR_MENU_TITLE.r, COLOR_MENU_TITLE.g, COLOR_MENU_TITLE.b, COLOR_MENU_TITLE.a});
    this->text_bottom = fonts->getTexture(renderer, _("use up, down, confirm and back to scroll and menu to go back"), FontType::NORMAL, {255, 255, 255, 255});

    this->loadCredits();
    this->empty_line_height = this->options->getShellSize()/4;
}

CreditsState::~CreditsState() {
    SDL_DestroyTexture(text_title);
    for (int i = 0; i < (int) this->texts.size(); i++) {
        SDL_DestroyTexture(texts[i]);
        texts[i] = NULL;
    }
}


void CreditsState::handleEvents(std::vector<Event> events) {
    for(Event event :events) {
        switch (event) {
            case Event::QUIT:
                this->next_state = State::EXIT;
                this->done = true;
                break;
            case Event::MENU:
                this->done = true;
                break;
            case Event::CANCEL:
                if (this->position == 0) {
                    this->done = true;
                }
                this->position -= this->last_line_visible - this->position;
                if (this->position <= 0) {
                    this->position = 0;
                }
                break;
            case Event::CONFIRM:
                if (this->last_line_visible >= (int) this->credits.size() - 2) {
                    this->done = true;
                    break;
                }
                this->position = this->last_line_visible + 1;
                break;
            case Event::DOWN:
                if (this->last_line_visible == (int) this->credits.size() - 1) {
                    break;
                }
                this->position += 1;
                if (this->position >= (int) this->credits.size()) {
                    this->position = (int) this->credits.size() -1;
                }
                this->freeUnusedTexts();
                break;
            case Event::UP:
                this->position -= 1;
                if (this->position <= 0) {
                    this->position = 0;
                }
                this->freeUnusedTexts();
                break;
            default:
                break;
        }
    }
    
}

void CreditsState::update() {
    this->theme.update();
}

void CreditsState::draw(SDL_Renderer * renderer) {
    this->theme.draw(renderer);

    // Draw title
    SDL_Rect rect_title = {this->options->getScreenWidth() / 2, this->options->getScreenHeight() / 8, 0, 0};
    SDL_QueryTexture(text_title, NULL, NULL, &rect_title.w, &rect_title.h);
    rect_title.x -= rect_title.w/2;
    rect_title.y -= rect_title.h/2;
    SDL_RenderCopy(renderer, text_title, NULL, &rect_title);

    // Draw bottom text
    SDL_Rect rect_bottom = {this->options->getScreenWidth()/2, this->options->getScreenHeight(), 0, 0};
    SDL_QueryTexture(text_bottom, NULL, NULL, &rect_bottom.w, &rect_bottom.h);
    rect_bottom.x -= rect_bottom.w/2;
    rect_bottom.y -= rect_bottom.h * 1.5;
    SDL_RenderCopy(renderer, text_bottom, NULL, &rect_bottom);

    // Draw options
    int current_y = this->options->getScreenHeight()/4;
    for(int i = this->position; i < (int) this->credits.size(); i++) {
        if (this->credits[i].empty()) {
            current_y += empty_line_height;
            continue;
        }
        if (this->texts[i] == NULL) {
            if (this->credits[i][0] == '#') {
                std::string current_title = this->credits[i];
                current_title.erase(0, 1);
                this->texts[i] = fonts->getTexture(renderer, current_title, FontType::NORMAL, {255, 255, 255, 255});
            } else {
                this->texts[i] = fonts->getTexture(renderer, this->credits[i], FontType::SMALL, {255, 255, 255, 255});
            }
        }

        SDL_Rect rect = {this->options->getScreenWidth()/2, current_y, 0, 0};
        SDL_QueryTexture(this->texts[i], NULL, NULL, &rect.w, &rect.h);
        rect.x -= rect.w/2;

        // Break loop if there is no space left to draw
        current_y += rect.h;
        if (current_y >= rect_bottom.y) {
            this->last_line_visible = i - 1;
            break;
        }

        // Render the option text
        SDL_RenderCopy(renderer, this->texts[i], NULL, &rect);

        // Also allow the last line to be set as the last line visible
        if (i == (int) this->credits.size() - 1) {
            this->last_line_visible = i;
        }
    }

    // Draw scroll bar
    SDL_Rect rect_scrollbar;
    rect_scrollbar.w = this->options->getShellSize() / 4;
    rect_scrollbar.x = this->options->getScreenWidth() - rect_scrollbar.w;
    rect_scrollbar.y = rect_title.y + rect_title.h;
    rect_scrollbar.h = rect_bottom.y - rect_scrollbar.y;

    SDL_SetRenderDrawColor(renderer, COLOR_BOARD.r, COLOR_BOARD.g, COLOR_BOARD.b, COLOR_BOARD.a);
    SDL_RenderFillRect(renderer, &rect_scrollbar);

    // Draw scroll bar
    SDL_Rect rect_pointer;
    rect_pointer.x = rect_scrollbar.x;
    rect_pointer.w = rect_scrollbar.w;
    rect_pointer.y = rect_scrollbar.y + ((float) rect_scrollbar.h / (float) this->credits.size() * this->position);
    rect_pointer.h = (float) rect_scrollbar.h / (float) this->credits.size() * (this->last_line_visible - this->position);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, COLOR_BOARD.a);
    SDL_RenderFillRect(renderer, &rect_pointer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(renderer,
        rect_scrollbar.x,
        rect_scrollbar.y,
        rect_scrollbar.x,
        rect_scrollbar.y + rect_scrollbar.h
    );
    SDL_RenderDrawLine(renderer,
        rect_scrollbar.x,
        rect_scrollbar.y,
        rect_scrollbar.x + rect_scrollbar.w,
        rect_scrollbar.y
    );
    SDL_RenderDrawLine(renderer,
        rect_scrollbar.x,
        rect_scrollbar.y + rect_scrollbar.h,
        rect_scrollbar.x + rect_scrollbar.w,
        rect_scrollbar.y + rect_scrollbar.h
    );
}

void CreditsState::loadCredits() {
    std::string credits_path = getResourcePath("assets/CREDITS.md");
    std::ifstream credits_file(credits_path);

    std::string line;
    if (credits_file.is_open()) {
        while(std::getline(credits_file, line)) {
            this->credits.push_back(line);
        }
    } else {
        SDL_Log("Error: Could not open CREDITS.md at %s", credits_path.c_str());
        this->credits.push_back(_("# could not open credits file"));
    }
    // Make sure the text texture array has the same amount of lines
    this->texts.reserve(this->credits.size());
    for(size_t i = 0; i < this->credits.size(); i++) {
        this->texts.push_back(NULL);
    }
}

void CreditsState::freeUnusedTexts() {
    if (this->position > cache_size) {
        for (int i = this->position - cache_size; i > 0; i--) {
            SDL_DestroyTexture(this->texts[i]);
            this->texts[i] = NULL;
        }
    }
    if (this->last_line_visible + cache_size < (int) this->credits.size()) {
        for (int i = this->last_line_visible + cache_size; i < (int) this->credits.size(); i++) {
            SDL_DestroyTexture(this->texts[i]);
            this->texts[i] = NULL;
        }
    }
}

State CreditsState::getNextState() {
    return this->next_state;
}