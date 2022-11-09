#include "MenuState.hpp"

#include "../constants.hpp"
#include "../colors.hpp"
#include "../FontType.hpp"
#include "GameState.hpp"

MenuState::MenuState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options) : renderer(renderer), fonts(fonts), sounds(sounds), options(options),
    theme(renderer, options, Theme::MENU)
{
    this->text_title = fonts->getTexture(renderer, "OceanPop", FontType::TITLE, {COLOR_MENU_TITLE.r, COLOR_MENU_TITLE.g, COLOR_MENU_TITLE.b, COLOR_MENU_TITLE.a});

    for (int i = 1; i < ((int) State::EXIT + 1); i++) {
        std::string option_text;
        std::string option_sub_text;
        switch ((State) i) {
            case State::STANDARD:
                option_text = "standard mode";
                option_sub_text = "play pre-made levels";
                break;
            case State::CHALLENGE:
                option_text = "challenge mode";
                option_sub_text = "ends only when lives run out";
                break;
            case State::RELAXED:
                option_text = "relaxed mode";
                option_sub_text = "match without consequences";
                break;
            case State::HIGHSCORES:
                option_text = "high scores";
                break;
            case State::OPTIONS:
                option_text = "options";
                break;
            case State::EXIT:
                option_text = "exit";
                break;
            default:
                option_text = "?????????";
                break;
        }
        texts.push_back(fonts->getTexture(renderer, option_text, FontType::NORMAL, {255, 255, 255, 255}));
        if (!option_sub_text.empty()) {
            sub_texts.push_back(fonts->getTexture(renderer, option_sub_text, FontType::SMALL, {255, 255, 255, 255}));
        } else {
            sub_texts.push_back(NULL);
        }
    }

    this->text_start_y = getOptionY(0);
}

MenuState::~MenuState() {
    SDL_DestroyTexture(text_title);
    for (int i = 0; i < (int) texts.size(); i++) {
        SDL_DestroyTexture(texts[i]);
        texts[i] = NULL;
    }
}


void MenuState::handleEvents(std::vector<Event> events) {
    SDL_Point mouse;

    for(Event event :events) {
        switch (event) {
            case Event::UP:
                if (selection != 0) {
                    selection--;
                } else {
                    selection = (int) texts.size() - 1;
                }
                break;
            case Event::DOWN:
                if (selection < ((int) texts.size() - 1)) {
                    selection++;
                } else {
                    selection = 0;
                }
                break;
            case Event::CONFIRM:
                this->done = true;
                break;
            case Event::QUIT:
                this->selection = (int) State::EXIT  - 1;
                this->done = true;
                break;
            case Event::MOUSEMOVE:
                SDL_GetMouseState(&mouse.x, &mouse.y);
                if (mouse.y >= this->text_start_y) {
                    this->selection = mouse.y/(this->options->getScreenHeight()/((int) this->texts.size() + text_offset)) - text_offset;
                }
                break;
            default:
                break;
        }
    }
    
}

void MenuState::update() {
    this->theme.update();
}

void MenuState::draw(SDL_Renderer * renderer) {
    this->theme.draw(renderer);

    // Draw title
    SDL_Rect rect_title = {this->options->getScreenWidth() / 2, this->options->getShellSize() / 2, 0, 0};
    SDL_QueryTexture(text_title, NULL, NULL, &rect_title.w, &rect_title.h);
    rect_title.x -= rect_title.w/2;
    SDL_RenderCopy(renderer, text_title, NULL, &rect_title);

    // Draw options
    for(int i = 0; i < (int) texts.size(); i++) {
        // Draw the option title
        SDL_Rect rect = {this->options->getScreenWidth()/2, getOptionY(i), 0, 0};
        SDL_QueryTexture(texts[i], NULL, NULL, &rect.w, &rect.h);
        rect.x -= rect.w/2;

        SDL_Rect sub_rect;
        if (sub_texts[i] != NULL) {
            sub_rect = {this->options->getScreenWidth()/2, rect.y+rect.h, 0, 0};
            SDL_QueryTexture(sub_texts[i], NULL, NULL, &sub_rect.w, &sub_rect.h);
            sub_rect.x -= sub_rect.w/2;
        }

        // Set the texture color
        if(i == selection) {
            // Draw selection box
            SDL_Rect rect_selection = {0, rect.y, this->options->getScreenWidth(), rect.h};
            SDL_SetRenderDrawColor(renderer, COLOR_BOARD.r, COLOR_BOARD.g, COLOR_BOARD.b, COLOR_BOARD.a);
            SDL_RenderFillRect(renderer, &rect_selection);

            // Draw lines around selection box
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawLine(renderer, rect_selection.x, rect_selection.y, rect_selection.x + rect_selection.w, rect_selection.y);
            SDL_RenderDrawLine(renderer, rect_selection.x, rect_selection.y + rect_selection.h, rect_selection.x + rect_selection.w, rect_selection.y + rect_selection.h);
        }

        // Render the option text
        SDL_RenderCopy(renderer, texts[i], NULL, &rect);
        if (sub_texts[i] != NULL && sub_rect.y + sub_rect.h < getOptionY(i+1)) {
            SDL_RenderCopy(renderer, sub_texts[i], NULL, &sub_rect);
        }
    }
}

int MenuState::getOptionY(int number) {
    return this->options->getScreenHeight()/(((int) texts.size())+this->text_offset)*(number+this->text_offset);
}

State MenuState::getNextState() {
    return (State) (this->selection + 1);
}