#include "GameOverState.hpp"

GameOverState::GameOverState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options) :
    theme(renderer, options, Theme::MENU),
    screen_text(renderer, fonts, options, "Game Over!", "press the confirm button to exit")
{
    this->renderer = renderer;
    this->fonts = fonts;
    this->sounds = sounds;
    this->options = options;
}

GameOverState::~GameOverState() {
}

void GameOverState::update() {
    if (!this->sound_played) {
        this->sounds->play(Sound::FAILED);
        this->sound_played = true;
    }
}

void GameOverState::handleEvents(std::vector<Event> events) {
    for(Event event: events) {
        if (event == Event::CONFIRM) {
            this->done = true;
        } else if (event == Event::QUIT) {
            this->next_state = State::EXIT;
            this->done = true;
        }
    }
}

void GameOverState::draw(SDL_Renderer *renderer) {
    this->theme.draw(renderer);
    screen_text.draw(renderer);
}

State GameOverState::getNextState() {
    return this->next_state;
}
