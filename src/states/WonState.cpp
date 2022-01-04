#include "WonState.hpp"

WonState::WonState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds) :
    theme(renderer, Theme::MENU),
    screen_text(renderer, "Game mode completed!", "Press the confirm button to exit")
{
    this->renderer = renderer;
    this->fonts = fonts;
    this->sounds = sounds;
}

WonState::~WonState() {
}

void WonState::update() {
    if (!this->sound_played) {
        this->sounds->play(Sound::COMPLETED);
        this->sound_played = true;
    }
}

void WonState::handleEvents(std::vector<Event> events) {
    for(Event event: events) {
        if (event == Event::CONFIRM) {
            this->done = true;
        }
    }
}

void WonState::draw(SDL_Renderer *renderer) {
    this->theme.draw(renderer);
    screen_text.draw(renderer);
}

State WonState::getNextState() {
    return State::MENU;
}