#include <iostream>
#include <stdexcept>
#include "Window.hpp"
#include "StateManager.hpp"
#include "utils.hpp"

void handle_input(Window &window, StateManager &stateManager) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            window.should_close = true;
        }
    }
}

void run() {
    Window window("Match Theory", 800, 600);
    StateManager stateManager;

    while (!window.should_close) {
        handle_input(window, stateManager);

        stateManager.update();

        window.clear();
        stateManager.draw();
        window.present();
    }
}

int main() {
    try {
        run();
    } catch (std::runtime_error &e) {
        std::cerr << "Panicked at '" << e.what() << "'." << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
