#include "EventManager.hpp"

std::vector<Event> EventManager::getEvents(Window &window) {
    std::vector<Event> inputs;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                window.should_close = true;
                break;
            case SDL_KEYDOWN:
                inputs.push_back(getEventFromKeyboard(event.key.keysym.sym));
                break;
        }
    }
    return inputs;
}

Event EventManager::getEventFromKeyboard(SDL_Keycode key) {
    switch (key) {
            case SDLK_w:
            case SDLK_UP:
                return Event::UP;
                break;
            case SDLK_s:
            case SDLK_DOWN:
                return Event::DOWN;
                break;
            case SDLK_a:
            case SDLK_LEFT:
                return Event::LEFT;
                break;
            case SDLK_d:
            case SDLK_RIGHT:
                return Event::RIGHT;
                break;
            case SDLK_e:
            case SDLK_RETURN:
            case SDLK_SPACE:
                return Event::CONFIRM;
                break;
            case SDLK_q:
            case SDLK_BACKSPACE:
                return Event::CANCEL;
                break;
            case SDLK_ESCAPE:
                return Event::MENU;
                break;
        }
}
