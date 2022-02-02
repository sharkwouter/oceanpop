#include "EventManager.hpp"

#include "constants.hpp"

#define AXIS_MAX 32767
#define AXIS_MIN -32767

EventManager::~EventManager() {
    closeAllGameControllers();
}

std::vector<Event> EventManager::getEvents() {
    std::vector<Event> inputs;
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        Event input = Event::NONE;

        switch (event.type) {
            case SDL_QUIT:
                input = Event::QUIT;
                break;
            case SDL_KEYDOWN:
                SDL_ShowCursor(0);
                input = getEventFromKeyboard(event.key.keysym.sym);
                break;
            case SDL_CONTROLLERBUTTONDOWN:
                SDL_ShowCursor(0);
                input = getEventFromControllerButton(event.cbutton.button);
                break;
            case SDL_MOUSEBUTTONDOWN:
                SDL_ShowCursor(1);
                input = getEventFromMouseButton(event.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                input = getEventFromMouseButtonUp(event.button.button);
                break;
            case SDL_MOUSEMOTION:
                SDL_ShowCursor(1);
                input = Event::MOUSEMOVE;
                break;
            case SDL_CONTROLLERAXISMOTION:
                SDL_ShowCursor(0);
                input = getEventFromControllerAxis(event.caxis.axis, event.caxis.value);
                break;
            case SDL_CONTROLLERDEVICEADDED:
                openGameController(event.cdevice.which);
                break;
            case SDL_CONTROLLERDEVICEREMOVED:
                closeDisconnectedGameControllers();
                break;
        }
        if (input != Event::NONE) {
            inputs.push_back(input);
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

Event EventManager::getEventFromMouseButton(Uint8 button) {
    Event event;

    switch (button) {
        case SDL_BUTTON_LEFT:
            event = Event::CONFIRM;
            break;
        case SDL_BUTTON_RIGHT:
            event = Event::CANCEL;
            break;
        default:
            event = Event::OTHER;
            break;
    }

    return event;
}

Event EventManager::getEventFromMouseButtonUp(Uint8 button) {
    Event event;

    switch (button) {
        default:
            event = Event::NONE;
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

Event EventManager::getEventFromControllerAxis(Uint32 axis, Sint16 value) {
    Event event = Event::NONE;

    switch (axis) {
        case SDL_CONTROLLER_AXIS_LEFTX:
            if (value > (AXIS_MAX*ANALOG_DEADZONE_MULTIPLIER)) {
                if (this->returned_to_horizontal_center) {
                    event = Event::RIGHT;
                    this->returned_to_horizontal_center = false;
                }
            } else if (value < (AXIS_MIN*ANALOG_DEADZONE_MULTIPLIER)) {
                if(this->returned_to_horizontal_center) {
                    event = Event::LEFT;
                    this->returned_to_horizontal_center = false;
                }
            } else {
                this->returned_to_horizontal_center = true;
            }
            break;
        case SDL_CONTROLLER_AXIS_LEFTY:
            if (value > (AXIS_MAX*ANALOG_DEADZONE_MULTIPLIER)) {
                if(this->returned_to_vertical_center) {
                    event = Event::DOWN;
                    this->returned_to_vertical_center = false;
                }
            } else if (value < (AXIS_MIN*ANALOG_DEADZONE_MULTIPLIER)) {
                if(this->returned_to_vertical_center) {
                    event = Event::UP;
                    this->returned_to_vertical_center = false;
                }
            } else {
                this->returned_to_vertical_center = true;
            }
            break;
        default:
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
    for (int i = 0; i < int(gameControllers.size()); i++) {
        SDL_Log("Removing controller: %s", SDL_GameControllerName(gameControllers[i]));
        SDL_GameControllerClose(gameControllers[i]);
        gameControllers[i] = NULL;
    }
}
