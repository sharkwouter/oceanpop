#include <iostream>
#include <stdexcept>
#include <vector>
#include "Window.hpp"
#include "StateManager.hpp"
#include "EventManager.hpp"
#include "TextureManager.hpp"
#include "utils.hpp"
#include "constants.hpp"
#include "states/GameState.hpp"

#ifdef __PSP__
    #include "pspkernel.h"
    PSP_MODULE_INFO("Match Theory", 0, 1, 0);
    PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);
#endif


void run() {
    Window window("Match Theory", SCREEN_WIDTH, SCREEN_HEIGHT);
    StateManager state_manager(new GameState(window.renderer));
    EventManager input_manager;
    TextureManager texture_manager;

    while (!window.should_close) {
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

    try {
        run();
    } catch (...) {
        std::rethrow_exception(std::current_exception());
    }

    return EXIT_SUCCESS;
}
