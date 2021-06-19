#include <vector>
#include <SDL2/SDL.h>

#include "Window.hpp"
#include "Event.hpp"

class EventManager {

private:
    Event getEventFromKeyboard(SDL_Keycode key);

    SDL_Event event;

public:
    std::vector<Event> getEvents(Window &window);

};