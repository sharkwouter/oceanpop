#include <iostream>
#include <stdexcept>
#include "Window.hpp"
#include "utils.hpp"

void run() {
    Window window("Match Theory", 800, 600);
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
