#include <iostream>
#include <stdexcept>
#include <vector>
#include "Window.hpp"
#include "FontManager.hpp"
#include "SoundManager.hpp"
#include "StateManager.hpp"
#include "EventManager.hpp"
#include "TextureManager.hpp"
#include "OptionManager.hpp"
#include "utils.hpp"
#include "constants.hpp"
#include "states/MenuState.hpp"

void run() {
    OptionManager option_manager;
    Window window("Match Theory", SCREEN_WIDTH, SCREEN_HEIGHT, &option_manager);
    FontManager font_manager;
    SoundManager sound_manager;
    StateManager state_manager(window.renderer, &font_manager, &sound_manager, &option_manager);
    EventManager input_manager;
    TextureManager texture_manager;

    while (!state_manager.isDone()) {
        std::vector<Event> events = input_manager.getEvents(window);

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

    run();

    return EXIT_SUCCESS;
}
