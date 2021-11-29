#include <vector>
#include <SDL2/SDL.h>

#include "Window.hpp"
#include "Event.hpp"

class EventManager {

private:
    bool returned_to_horizontal_center;
    bool returned_to_vertical_center;

    Event getEventFromKeyboard(SDL_Keycode key);
    Event getEventFromControllerButton(Uint32 button);
    Event getEventFromControllerAxis(Uint32 axis, Sint16 value);

    void openGameController(Sint32);
    void closeDisconnectedGameControllers();
    void closeAllGameControllers();

    std::vector<SDL_GameController*> gameControllers;

public:
    ~EventManager();

    std::vector<Event> getEvents(Window &window);

};