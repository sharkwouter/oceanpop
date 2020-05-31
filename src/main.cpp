#include <iostream>
#include <stdexcept>
#include "Window.hpp"
#include "utils.hpp"

void run() {
    Window window("Match Theory", 800, 600);

    while (!window.should_close) {
        window.handle_input();
        window.clear();
        // Draw here
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
