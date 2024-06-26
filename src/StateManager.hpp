#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP

#include <SDL.h>
#include <vector>
#include "states/BaseState.hpp"
#include "Event.hpp"
#include "FontManager.hpp"
#include "SoundManager.hpp"
#include "OptionManager.hpp"

class StateManager {
private:
    SDL_Renderer * renderer;
    FontManager * fonts;
    SoundManager * sounds;
    OptionManager * options;
    SDL_Window * window;

    BaseState * state = NULL;

    bool done = false;
    void clearState();
    void switchState();
public:
    StateManager(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options, SDL_Window * window);
    ~StateManager();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer *renderer);

    bool isDone() {return done;};
};

#endif // STATEMANAGER_HPP
