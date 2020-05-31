#include "Window.hpp"
#include "utils.hpp"

Window::Window(const std::string &title, int width, int height) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        panic("couldn't init SDL: " + std::string(SDL_GetError()));
    }

    this->window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (this->window == nullptr) {
        panic("couldn't create window: " + std::string(SDL_GetError()));
    }
}

Window::~Window() {
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}
