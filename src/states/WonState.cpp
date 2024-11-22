#include "WonState.hpp"

#include "../utils.hpp"

WonState::WonState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options) :
    theme(renderer, options, Theme::MENU),
    screen_text(renderer, fonts, options, _("Game Mode Completed!"), _("press the confirm button to exit"))
{
    this->renderer = renderer;
    this->fonts = fonts;
    this->sounds = sounds;
    this->options = options;
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
        } else if (event == Event::QUIT) {
            this->next_state = State::EXIT;
            this->done = true;
        } else if (event == Event::WINDOW_RESIZE) {
            this->screen_text.updateSizing();
        }
    }
}

void WonState::draw(SDL_Renderer *renderer) {
    this->theme.draw(renderer);
    screen_text.draw(renderer);
}

State WonState::getNextState() {
    return this->next_state;
}
