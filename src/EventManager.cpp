#include "EventManager.hpp"

EventManager::~EventManager() {
    closeAllGameControllers();
}

std::vector<Event> EventManager::getEvents(Window &window) {
    std::vector<Event> inputs;
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                window.should_close = true;
                break;
            case SDL_KEYDOWN:
                inputs.push_back(getEventFromKeyboard(event.key.keysym.sym));
                break;
            case SDL_CONTROLLERBUTTONDOWN:
                inputs.push_back(getEventFromControllerButton(event.cbutton.button));
                break;
            case SDL_CONTROLLERDEVICEADDED:
                openGameController(event.cdevice.which);
                break;
            case SDL_CONTROLLERDEVICEREMOVED:
                closeDisconnectedGameControllers();
                break;
        }
    }
    return inputs;
}

Event EventManager::getEventFromKeyboard(SDL_Keycode key) {
    Event event;

    switch (key) {
        case SDLK_w:
        case SDLK_UP:
            event = Event::UP;
            break;
        case SDLK_s:
        case SDLK_DOWN:
            event = Event::DOWN;
            break;
        case SDLK_a:
        case SDLK_LEFT:
            event = Event::LEFT;
            break;
        case SDLK_d:
        case SDLK_RIGHT:
            event = Event::RIGHT;
            break;
        case SDLK_e:
        case SDLK_RETURN:
        case SDLK_SPACE:
            event = Event::CONFIRM;
            break;
        case SDLK_q:
        case SDLK_BACKSPACE:
            event = Event::CANCEL;
            break;
        case SDLK_ESCAPE:
            event = Event::MENU;
            break;
        default:
            event = Event::OTHER;
            break;
    }

    return event;
}

Event EventManager::getEventFromControllerButton(Uint32 button) {
    Event event;

    switch (button) {
        case SDL_CONTROLLER_BUTTON_DPAD_UP:
            event = Event::UP;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            event = Event::DOWN;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
            event = Event::LEFT;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            event = Event::RIGHT;
            break;
        case SDL_CONTROLLER_BUTTON_A:
            event = Event::CONFIRM;
            break;
        case SDL_CONTROLLER_BUTTON_B:
            event = Event::CANCEL;
            break;
        case SDL_CONTROLLER_BUTTON_START:
            event = Event::MENU;
            break;
        default:
            event = Event::OTHER;
            break;
        }

    return event;
}

void EventManager::openGameController(Sint32 index) {
     if (SDL_IsGameController(index)) {
        SDL_GameController * controller = SDL_GameControllerOpen(index);
        SDL_Log("Adding controller: %s", SDL_GameControllerName(controller));
        gameControllers.push_back(controller);
    }
}

void EventManager::closeDisconnectedGameControllers() {
    std::vector<SDL_GameController*> currentControllers;
    for(SDL_GameController * controller : gameControllers) {
        if (!SDL_GameControllerGetAttached(controller)) {
            SDL_Log("Removing controller: %s", SDL_GameControllerName(controller));
            SDL_GameControllerClose(controller);
            controller = NULL;
        } else {
            currentControllers.push_back(controller);
        }
    }

    gameControllers = currentControllers;
}

void EventManager::closeAllGameControllers() {
    for (int i = 0; i < gameControllers.size(); i++) {
        SDL_Log("Removing controller: %s", SDL_GameControllerName(gameControllers[i]));
        SDL_GameControllerClose(gameControllers[i]);
        gameControllers[i] = NULL;
    }
}
