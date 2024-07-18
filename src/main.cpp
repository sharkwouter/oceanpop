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
#include "TranslationManager.hpp"
#include "utils.hpp"
#include "constants.hpp"
#include "states/MenuState.hpp"

#ifdef TRANSLATION_SUPPORT
    // Create global TranslationManager object set in utils.h
    TranslationManager translation_manager;
#endif

void run() {
    OptionManager option_manager;
    Window window("OceanPop", &option_manager);
    FontManager font_manager(&option_manager);
    SoundManager sound_manager(&option_manager);
    StateManager state_manager(window.renderer, &font_manager, &sound_manager, &option_manager, window.window);
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
    } catch (const std::runtime_error &e) {
        SDL_Log("Error: %s", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
