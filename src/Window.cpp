#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Window.hpp"
#include "utils.hpp"

Window::Window(const std::string &title, int width, int height, OptionManager * options) {
    #ifdef __vita__
        // Disable back touch
        SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS_DIRECT_ONLY, "1");
    #endif

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER | SDL_INIT_TIMER) != 0) {
        panic("couldn't init SDL: " + std::string(SDL_GetError()));
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        panic("couldn't init SDL_image: " + std::string(IMG_GetError()));
    }

     if (TTF_Init() == -1) {
        panic("couldn't init SDL_ttf: " + std::string(TTF_GetError()));
    }

    if( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1 ) {
		panic("couldn't init SDL_mixer: " + std::string(Mix_GetError()));
    }

    this->window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (this->window == nullptr) {
        panic("couldn't create window: " + std::string(SDL_GetError()));
    }

    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (this->renderer == nullptr) {
        panic("couldn't create renderer: " + std::string(SDL_GetError()));
    }

    // Make the use of transparancy possible
    SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_BLEND);

    this->start_frame = SDL_GetTicks();
    this->frame_delay = 1000 / 60; // Target 60 fps

    if (options->getFullscreen()) {
        SDL_SetWindowFullscreen(this->window, SDL_WINDOW_FULLSCREEN);
    }
}

void Window::clear() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(this->renderer);
}

void Window::present() {
    int delta = SDL_GetTicks() - this->start_frame;
    if (delta < this->frame_delay) {
        SDL_Delay(this->frame_delay - delta);
    }
    SDL_RenderPresent(this->renderer);
    this->start_frame = SDL_GetTicks();
}

Window::~Window() {
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
