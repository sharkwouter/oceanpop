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
            case SDL_EVENT_QUIT:
                input = Event::QUIT;
                break;
            case SDL_EVENT_KEY_DOWN:
                SDL_HideCursor();
                input = getEventFromKeyboard(event.key.key);
                break;
            case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
                SDL_HideCursor();
                input = getEventFromControllerButton(event.gbutton.button);
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                SDL_ShowCursor();
                input = getEventFromMouseButton(event.button.button);
                break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                input = getEventFromMouseButtonUp(event.button.button);
                break;
            case SDL_EVENT_MOUSE_MOTION:
                SDL_ShowCursor();
                input = Event::MOUSEMOVE;
                break;
            case SDL_EVENT_GAMEPAD_AXIS_MOTION:
                SDL_ShowCursor();
                input = getEventFromControllerAxis(event.gaxis.axis, event.gaxis.value);
                break;
            case SDL_EVENT_GAMEPAD_ADDED:
                openGameController(event.cdevice.which);
                break;
            case SDL_EVENT_GAMEPAD_REMOVED:
                closeDisconnectedGameControllers();
                break;
            case SDL_EVENT_WINDOW_RESIZED:
                input = Event::WINDOW_RESIZE;
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
        case SDLK_W:
        case SDLK_UP:
            event = Event::UP;
            break;
        case SDLK_S:
        case SDLK_DOWN:
            event = Event::DOWN;
            break;
        case SDLK_A:
        case SDLK_LEFT:
            event = Event::LEFT;
            break;
        case SDLK_D:
        case SDLK_RIGHT:
            event = Event::RIGHT;
            break;
        case SDLK_E:
        case SDLK_RETURN:
        case SDLK_SPACE:
            event = Event::CONFIRM;
            break;
        case SDLK_Q:
        case SDLK_BACKSPACE:
            event = Event::CANCEL;
            break;
        case SDLK_PAGEUP:
            event = Event::NEXT;
            break;
        case SDLK_PAGEDOWN:
            event = Event::PREVIOUS;
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
        case SDL_BUTTON_MIDDLE:
            event = Event::MENU;
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
        case SDL_GAMEPAD_BUTTON_DPAD_UP:
            event = Event::UP;
            break;
        case SDL_GAMEPAD_BUTTON_DPAD_DOWN:
            event = Event::DOWN;
            break;
        case SDL_GAMEPAD_BUTTON_DPAD_LEFT:
            event = Event::LEFT;
            break;
        case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:
            event = Event::RIGHT;
            break;
        case SDL_GAMEPAD_BUTTON_SOUTH:
            event = Event::CONFIRM;
            break;
        case SDL_GAMEPAD_BUTTON_EAST:
            event = Event::CANCEL;
            break;
        case SDL_GAMEPAD_BUTTON_START:
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
        case SDL_GAMEPAD_AXIS_LEFTX:
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
        case SDL_GAMEPAD_AXIS_LEFTY:
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
     if (SDL_IsGamepad(index)) {
        SDL_Gamepad * controller = SDL_OpenGamepad(index);
        SDL_Log("Adding controller: %s", SDL_GetGamepadName(controller));
        gameControllers.push_back(controller);
    }
}

void EventManager::closeDisconnectedGameControllers() {
    std::vector<SDL_Gamepad*> currentControllers;
    for(SDL_Gamepad * controller : gameControllers) {
        if (!SDL_GamepadConnected(controller)) {
            SDL_Log("Removing controller: %s", SDL_GetGamepadName(controller));
            SDL_CloseGamepad(controller);
            controller = NULL;
        } else {
            currentControllers.push_back(controller);
        }
    }

    gameControllers = currentControllers;
}

void EventManager::closeAllGameControllers() {
    for (int i = 0; i < int(gameControllers.size()); i++) {
        SDL_Log("Removing controller: %s", SDL_GetGamepadName(gameControllers[i]));
        SDL_CloseGamepad(gameControllers[i]);
        gameControllers[i] = NULL;
    }
}
