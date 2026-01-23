#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Window.hpp"
#include "utils.hpp"
#include "constants.hpp"

Window::Window(const std::string &title, OptionManager * options) : options(options) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER | SDL_INIT_TIMER) != 0) {
        panic("couldn't init SDL: " + std::string(SDL_GetError()));
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        panic("couldn't init SDL_image: " + std::string(IMG_GetError()));
    }

     if (TTF_Init() == -1) {
        panic("couldn't init SDL_ttf: " + std::string(TTF_GetError()));
    }

    if( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1 ) {
		panic("couldn't init SDL_mixer: " + std::string(Mix_GetError()));
    }

    // Set default screen resolution and refresh rate if they haven't been set yet
    if (this->options->getScreenWidth() == 0 || this->options->getScreenHeight() == 0 ) {
        SDL_DisplayMode mode = getStandardDisplayMode();
        this->options->setScreenResolution(mode.w, mode.h);
    }

    Uint32 window_flags = SDL_WINDOW_RESIZABLE;
    if (options->getFullscreen()) {
        window_flags |= SDL_WINDOW_FULLSCREEN;
    }
    this->window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, options->getScreenWidth(), options->getScreenHeight(), window_flags);
    if (this->window == nullptr) {
        panic("couldn't create window: " + std::string(SDL_GetError()));
    }

    // Set the refresh rate
    if (this->options->getFullscreen()) {
        for(SDL_DisplayMode mode : getDisplayModes()) {
            if (mode.w == options->getScreenWidth() && mode.h == options->getScreenHeight()) {
                SDL_SetWindowDisplayMode(window, &mode);
                break;
            }
        }
    }
#ifdef __PS2__
    SDL_SetHint(SDL_HINT_PS2_DYNAMIC_VSYNC, "1");    
#endif
    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (this->renderer == nullptr) {
        panic("couldn't create renderer: " + std::string(SDL_GetError()));
    }

    // Make the use of transparancy possible
    SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_BLEND);

    this->start_frame = SDL_GetTicks();
    this->frame_delay = 1000 / 60; // Target 60 fps
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

void Window::updateSize() {
    int width, height;
    SDL_GetWindowSize(this->window, &width, &height);
    if (width < MIN_SCREEN_WIDTH) {
        width = MIN_SCREEN_WIDTH;
    }
    if (height < MIN_SCREEN_HEIGHT) {
        height = MIN_SCREEN_HEIGHT;
    }
    this->options->setScreenResolution(width, height);
}

Window::~Window() {
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    Mix_Quit();
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
