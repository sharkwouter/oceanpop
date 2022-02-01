#include <iostream>
#include <stdexcept>
#include <vector>

#include <SDL.h>
#include <json/json.h>

#include "Window.hpp"
#include "FontManager.hpp"
#include "SoundManager.hpp"
#include "StateManager.hpp"
#include "EventManager.hpp"
#include "OptionManager.hpp"
#include "utils.hpp"
#include "constants.hpp"
#include "states/MenuState.hpp"

void run() {
    OptionManager option_manager;
    Window window("OceanPop", &option_manager);
    FontManager font_manager;
    SoundManager sound_manager(&option_manager);
    StateManager state_manager(window.renderer, &font_manager, &sound_manager, &option_manager);
    EventManager input_manager;

    while (!state_manager.isDone()) {
        std::vector<Event> events = input_manager.getEvents();

        state_manager.handleEvents(events);
        state_manager.update();

        window.clear();
        state_manager.draw(window.renderer);
        window.present();
    }
}

int main(int argv, char** args) {
    (void) argv;
    (void) args;

    try {
        run();
    } catch (const Json::LogicError&) {
        SDL_Log("Configuration file invalid, please delete it");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
