#include "MenuState.hpp"

#include "../constants.hpp"
#include "../colors.hpp"
#include "GameState.hpp"

MenuState::MenuState(SDL_Renderer * renderer) : renderer(renderer), theme(renderer, Theme::MENU) {
    text_title = fonts.getTexture(renderer, "Match Theory", true, {255, 255, 255, 255});

    for (int i = 0; i < (int) MenuOption::OPTIONCOUNT; i++) {
        std::string option_text;
        switch ((MenuOption) i) {
            case MenuOption::STANDARD:
                option_text = "Standard mode";
                break;
            case MenuOption::RELAXED:
                option_text = "Relaxed mode";
                break;
            case MenuOption::OPTIONS:
                option_text = "Options";
                break;
            case MenuOption::EXIT:
                option_text = "Exit";
                break;
            default:
                option_text = "?????????";
                break;
        }
        options.push_back(fonts.getTexture(renderer, option_text,false, {255, 255, 255, 255}));
    }
}

MenuState::~MenuState() {
    SDL_DestroyTexture(text_title);
    for (SDL_Texture * texture : options) {
        SDL_DestroyTexture(texture);
    }
}


void MenuState::handleEvents(std::vector<Event> events) {
    for(Event event :events) {
        switch (event) {
            case Event::UP:
                current_option = (current_option - 1) % options.size(); 
                break;
            case Event::DOWN:
                current_option = (current_option + 1) % options.size(); 
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
    SDL_Rect rect_title = {SCREEN_WIDTH/2, SHELL_SIZE, 0, 0};
    SDL_QueryTexture(text_title, NULL, NULL, &rect_title.w, &rect_title.h);
    rect_title.x -= rect_title.w/2;
    rect_title.y -= rect_title.h/2;
    SDL_RenderCopy(renderer, text_title, NULL, &rect_title);

    // Draw options
    for(int i = 0; i < (int) options.size(); i++) {
        // Set the texture color
        if(i == current_option) {
            SDL_SetTextureColorMod(options[i], 0, 0, 0);
        } else {
            SDL_SetTextureColorMod(options[i], 255, 255, 255);
        }
        // Draw the option title
        SDL_Rect rect = {SCREEN_WIDTH/2, (SCREEN_HEIGHT/(options.size()+2))*(i+2), 0, 0};
        SDL_QueryTexture(options[i], NULL, NULL, &rect.w, &rect.h);
        rect.x -= rect.w/2;
        rect.y -= rect.h/2;
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
            return State::EXIT;
            break;
    }
}