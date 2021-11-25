#include <vector>
#include <SDL2/SDL.h>

#include "Window.hpp"
#include "Event.hpp"

class EventManager {

private:
    Event getEventFromKeyboard(SDL_Keycode key);
    Event getEventFromControllerButton(Uint32 button);

    void openGameController(Sint32);
    void closeDisconnectedGameControllers();
    void closeAllGameControllers();

    std::vector<SDL_GameController*> gameControllers;

public:
    ~EventManager();

    std::vector<Event> getEvents(Window &window);

};