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

    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (this->renderer == nullptr) {
        panic("couldn't create renderer: " + std::string(SDL_GetError()));
    }

    this->should_close = false;
}

void Window::handle_input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            this->should_close = true;
        }
    }
}

void Window::update() {
    // Do nothing.
}

void Window::draw() {
    SDL_RenderClear(this->renderer);
    SDL_RenderPresent(this->renderer);
}

Window::~Window() {
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}
