#include "CreditsState.hpp"

#include "../constants.hpp"
#include "../colors.hpp"
#include "../FontType.hpp"
#include "../utils.hpp"
#include "GameState.hpp"

CreditsState::CreditsState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options) : renderer(renderer), fonts(fonts), sounds(sounds), options(options),
    theme(renderer, options, Theme::MENU)
{
    this->text_title = fonts->getTexture(renderer, _("Credits"), FontType::TITLE, {COLOR_MENU_TITLE.r, COLOR_MENU_TITLE.g, COLOR_MENU_TITLE.b, COLOR_MENU_TITLE.a});
    this->text_bottom = fonts->getTexture(renderer, _("press confirm for next, cancel to go back"), FontType::NORMAL, {255, 255, 255, 255});

    std::string standard_mode_completed = options->getStandardModeCompleted() ? _("yes") : _("no");
    texts.push_back(fonts->getTexture(renderer, _("standard mode completed: ") + standard_mode_completed, FontType::SMALL, {255, 255, 255, 255}));
    texts.push_back(fonts->getTexture(renderer, _("highest level reached in challenge mode: ") + std::to_string(options->getChallengeModeHighscore()), FontType::SMALL, {255, 255, 255, 255}));
    texts.push_back(fonts->getTexture(renderer, _("matches in relaxed mode: ") + std::to_string(options->getRelaxedModeScore()), FontType::SMALL, {255, 255, 255, 255}));

    this->text_start_y = getTextY(0);
}

CreditsState::~CreditsState() {
    SDL_DestroyTexture(text_title);
    for (int i = 0; i < (int) texts.size(); i++) {
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
            case Event::CANCEL:
            case Event::CONFIRM:
                this->done = true;
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

    // Draw options
    for(int i = 0; i < (int) texts.size(); i++) {
        // Draw the option title
        SDL_Rect rect = {this->options->getScreenWidth()/2, getTextY(i), 0, 0};
        SDL_QueryTexture(texts[i], NULL, NULL, &rect.w, &rect.h);
        rect.x -= rect.w/2;

        // Render the option text
        SDL_RenderCopy(renderer, texts[i], NULL, &rect);
    }

    SDL_Rect rect_bottom = {this->options->getScreenWidth()/2, getTextY(((int) texts.size()) + 1), 0, 0};
    SDL_QueryTexture(text_bottom, NULL, NULL, &rect_bottom.w, &rect_bottom.h);
    rect_bottom.x -= rect_bottom.w/2;
    SDL_RenderCopy(renderer, text_bottom, NULL, &rect_bottom);
}

int CreditsState::getTextY(int number) {
    return this->options->getScreenHeight()/(((int) texts.size())+this->text_offset*2)*(number+this->text_offset);
}

State CreditsState::getNextState() {
    return this->next_state;
}