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

#if defined(_WIN32)
int main(int argv, char** args) {
#elif defined(__PSP__)
int SDL_main(int argc, char *argv[]) {
#else
int main() {
#endif
    try {
        run();
    } catch (...) {
        std::rethrow_exception(std::current_exception());
    }

    return EXIT_SUCCESS;
}
