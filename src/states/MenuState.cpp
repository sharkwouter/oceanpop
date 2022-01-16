#include "MenuState.hpp"

#include "../constants.hpp"
#include "../colors.hpp"
#include "GameState.hpp"

MenuState::MenuState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds) : renderer(renderer), fonts(fonts), sounds(sounds),
    theme(renderer, Theme::MENU)
{
    this->text_title = fonts->getTexture(renderer, "OceanPop", true, {COLOR_MENU_TITLE.r, COLOR_MENU_TITLE.g, COLOR_MENU_TITLE.b, COLOR_MENU_TITLE.a});

    for (int i = 1; i < ((int) State::EXIT + 1); i++) {
        std::string option_text;
        switch ((State) i) {
            case State::STANDARD:
                option_text = "Standard mode";
                break;
            case State::CHALLENGE:
                option_text = "Challenge mode";
                break;
            case State::RELAXED:
                option_text = "Relaxed mode";
                break;
            case State::HIGHSCORES:
                option_text = "High scores";
                break;
            case State::OPTIONS:
                option_text = "Options";
                break;
            case State::EXIT:
                option_text = "Exit";
                break;
            default:
                option_text = "?????????";
                break;
        }
        options.push_back(fonts->getTexture(renderer, option_text,false, {255, 255, 255, 255}));
    }

    this->options_start_y = getOptionY(0);
}

MenuState::~MenuState() {
    SDL_DestroyTexture(text_title);
    for (int i = 0; i < (int) options.size(); i++) {
        SDL_DestroyTexture(options[i]);
        options[i] = NULL;
    }
}


void MenuState::handleEvents(std::vector<Event> events) {
    SDL_Point mouse;

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
            case Event::QUIT:
            case Event::CANCEL:
                this->current_option = (int) State::EXIT  - 1;
                this->done = true;
                break;
            case Event::MOUSEMOVE:
                SDL_GetMouseState(&mouse.x, &mouse.y);
                if (mouse.y >= this->options_start_y) {
                    this->current_option = mouse.y/(SCREEN_HEIGHT/((int) this->options.size() + options_offset)) - options_offset;
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
    SDL_Rect rect_title = {SCREEN_WIDTH / 2, SHELL_SIZE / 2, 0, 0};
    SDL_QueryTexture(text_title, NULL, NULL, &rect_title.w, &rect_title.h);
    rect_title.x -= rect_title.w/2;
    SDL_RenderCopy(renderer, text_title, NULL, &rect_title);

    // Draw options
    for(int i = 0; i < (int) options.size(); i++) {
        // Draw the option title
        SDL_Rect rect = {SCREEN_WIDTH/2, getOptionY(i), 0, 0};
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

int MenuState::getOptionY(int number) {
    return SCREEN_HEIGHT/(((int) options.size())+this->options_offset)*(number+this->options_offset);
}

State MenuState::getNextState() {
    return (State) (this->current_option + 1);
}