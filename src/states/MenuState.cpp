#include "MenuState.hpp"

#include "../constants.hpp"
#include "../colors.hpp"
#include "GameState.hpp"

MenuState::MenuState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds) : renderer(renderer), fonts(fonts), sounds(sounds),
    theme(renderer, Theme::MENU)
{
    text_title = fonts->getTexture(renderer, "Match Theory", true, {COLOR_MENU_TITLE.r, COLOR_MENU_TITLE.g, COLOR_MENU_TITLE.b, COLOR_MENU_TITLE.a});

    for (int i = 0; i < (int) MenuOption::OPTIONCOUNT; i++) {
        std::string option_text;
        switch ((MenuOption) i) {
            case MenuOption::STANDARD:
                option_text = "Standard mode";
                break;
            case MenuOption::ENDLESS:
                option_text = "Endless mode";
                break;
            case MenuOption::RELAXED:
                option_text = "Relaxed mode";
                break;
            case MenuOption::HOWTOPLAY:
                option_text = "How to play";
                break;
            case MenuOption::HIGHSCORES:
                option_text = "High scores";
                break;
            case MenuOption::OPTIONS:
                option_text = "Options";
                break;
            case MenuOption::CREDITS:
                option_text = "Credits";
                break;
            case MenuOption::EXIT:
                option_text = "Exit";
                break;
            default:
                option_text = "?????????";
                break;
        }
        options.push_back(fonts->getTexture(renderer, option_text,false, {255, 255, 255, 255}));
    }
}

MenuState::~MenuState() {
    SDL_DestroyTexture(text_title);
    for (int i = 0; i < (int) options.size(); i++) {
        SDL_DestroyTexture(options[i]);
        options[i] = NULL;
    }
}


void MenuState::handleEvents(std::vector<Event> events) {
    for(Event event :events) {
        switch (event) {
            case Event::UP:
                if (current_option != 0) {
                    current_option--;
                } else {
                    current_option = (int) options.size() - 1;
                }
                break;
            case Event::DOWN:
                if (current_option < ((int) options.size() - 1)) {
                    current_option++;
                } else {
                    current_option = 0;
                }
                break;
            case Event::CONFIRM:
                this->done = true;
                break;
            case Event::CANCEL:
                this->current_option = (int) MenuOption::EXIT;
                this->done = true;
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
    SDL_Rect rect_title = {SCREEN_WIDTH / 2, SHELL_SIZE / 2, 0, 0};
    SDL_QueryTexture(text_title, NULL, NULL, &rect_title.w, &rect_title.h);
    rect_title.x -= rect_title.w/2;
    SDL_RenderCopy(renderer, text_title, NULL, &rect_title);

    // Draw options
    for(int i = 0; i < (int) options.size(); i++) {
        // Draw the option title
        SDL_Rect rect = {SCREEN_WIDTH/2, (SCREEN_HEIGHT/((int) options.size()+3))*(i+3), 0, 0};
        SDL_QueryTexture(options[i], NULL, NULL, &rect.w, &rect.h);
        rect.x -= rect.w/2;

        // Set the texture color
        if(i == current_option) {
            // Draw selection box
            SDL_Rect rect_selection = {0, rect.y, SCREEN_WIDTH, rect.h};
            SDL_SetRenderDrawColor(renderer, COLOR_BOARD.r, COLOR_BOARD.g, COLOR_BOARD.b, COLOR_BOARD.a);
            SDL_RenderFillRect(renderer, &rect_selection);

            // Draw lines around selection box
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawLine(renderer, rect_selection.x, rect_selection.y, rect_selection.x + rect_selection.w, rect_selection.y);
            SDL_RenderDrawLine(renderer, rect_selection.x, rect_selection.y + rect_selection.h, rect_selection.x + rect_selection.w, rect_selection.y + rect_selection.h);
        }

        // Render the option text
        SDL_RenderCopy(renderer, options[i], NULL, &rect);
    }
}

State MenuState::getNextState() {
    switch ((MenuOption) this->current_option) {
        case MenuOption::EXIT:
            return State::EXIT;
            break;
        case MenuOption::STANDARD:
            return State::STANDARD;
        default:
            return State::MENU;
            break;
    }
}