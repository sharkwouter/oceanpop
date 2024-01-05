#include "HighscoreState.hpp"

#include "../constants.hpp"
#include "../colors.hpp"
#include "../FontType.hpp"
#include "GameState.hpp"

HighscoreState::HighscoreState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options) : renderer(renderer), fonts(fonts), sounds(sounds), options(options),
    theme(renderer, options, Theme::MENU)
{
    this->text_title = fonts->getTexture(renderer, "High Scores", FontType::TITLE, {COLOR_MENU_TITLE.r, COLOR_MENU_TITLE.g, COLOR_MENU_TITLE.b, COLOR_MENU_TITLE.a});
    this->text_bottom = fonts->getTexture(renderer, "press confirm to go back", FontType::NORMAL, {255, 255, 255, 255});

    std::string standard_mode_completed = options->getStandardModeCompleted() ? "yes" : "no";
    texts.push_back(fonts->getTexture(renderer, "standard mode completed: " + standard_mode_completed, FontType::NORMAL, {255, 255, 255, 255}));
    texts.push_back(fonts->getTexture(renderer, "highest level reached in challenge mode: " + std::to_string(options->getChallengeModeHighscore()), FontType::NORMAL, {255, 255, 255, 255}));
    texts.push_back(fonts->getTexture(renderer, "matches in relaxed mode: " + std::to_string(options->getRelaxedModeScore()), FontType::NORMAL, {255, 255, 255, 255}));

    this->text_start_y = getTextY(0);
}

HighscoreState::~HighscoreState() {
    SDL_DestroyTexture(text_title);
    for (int i = 0; i < (int) texts.size(); i++) {
        SDL_DestroyTexture(texts[i]);
        texts[i] = NULL;
    }
}


void HighscoreState::handleEvents(std::vector<Event> events) {
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

void HighscoreState::update() {
    this->theme.update();
}

void HighscoreState::draw(SDL_Renderer * renderer) {
    this->theme.draw(renderer);

    // Draw title
    SDL_Rect rect_title = {this->options->getScreenWidth() / 2, this->options->getShellSize() / 2, 0, 0};
    SDL_QueryTexture(text_title, NULL, NULL, &rect_title.w, &rect_title.h);
    rect_title.x -= rect_title.w/2;
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

int HighscoreState::getTextY(int number) {
    return this->options->getScreenHeight()/(((int) texts.size())+this->text_offset*2)*(number+this->text_offset);
}

State HighscoreState::getNextState() {
    return this->next_state;
}