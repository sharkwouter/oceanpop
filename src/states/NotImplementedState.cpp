#include "NotImplementedState.hpp"

NotImplementedState::NotImplementedState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds) :
    theme(renderer, Theme::MENU),
    screen_text(renderer, "Not yet implemented", "Press the confirm button to continue")
{
    this->renderer = renderer;
    this->fonts = fonts;
    this->sounds = sounds;
}

NotImplementedState::~NotImplementedState() {
}

void NotImplementedState::update() {

}

void NotImplementedState::handleEvents(std::vector<Event> events) {
    for(Event event: events) {
        if (event == Event::CONFIRM || event == Event::CANCEL || event == Event::MENU) {
            this->done = true;
        }
    }
}

void NotImplementedState::draw(SDL_Renderer *renderer) {
    this->theme.draw(renderer);
    screen_text.draw(renderer);
}

State NotImplementedState::getNextState() {
    return State::MENU;
}