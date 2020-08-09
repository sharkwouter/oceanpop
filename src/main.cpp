#include <iostream>
#include <stdexcept>
#include "Window.hpp"
#include "StateManager.hpp"
#include "TextureManager.hpp"
#include "utils.hpp"
#include "constants.hpp"
#include "GameState.hpp"

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
    Window window("Match Theory", SCREEN_WIDTH, SCREEN_HEIGHT);
    StateManager state_manager(new GameState());
    TextureManager texture_manager;

    while (!window.should_close) {
        handle_input(window, state_manager);

        state_manager.update();

        window.clear();
        state_manager.draw();
        window.present();
    }
}

int main() {
    try {
        run();
    } catch (...) {
        std::rethrow_exception(std::current_exception());
    }

    return EXIT_SUCCESS;
}
