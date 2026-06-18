#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
#include "Window.hpp"
#include "utils.hpp"
#include "constants.hpp"

Window::Window(const std::string &title, OptionManager * options) : options(options) {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD)) {
        panic("couldn't init SDL: " + std::string(SDL_GetError()));
    }

     if (!TTF_Init()) {
        panic("couldn't init SDL_ttf: " + std::string(SDL_GetError()));
    }

    if(!MIX_Init()) {
		panic("couldn't init SDL_mixer: " + std::string(SDL_GetError()));
    }

    // Set default screen resolution and refresh rate if they haven't been set yet
    if (this->options->getScreenWidth() == 0 || this->options->getScreenHeight() == 0 ) {
        SDL_DisplayMode * mode = getStandardDisplayMode();
        this->options->setScreenResolution(mode->w, mode->h);
    }

    Uint32 window_flags = SDL_WINDOW_RESIZABLE;
    if (options->getFullscreen()) {
        window_flags |= SDL_WINDOW_FULLSCREEN;
    }

    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, title.c_str());
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, options->getScreenWidth());
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, options->getScreenHeight());
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_FLAGS_NUMBER, window_flags);
    this->window =  SDL_CreateWindowWithProperties(props);
    if (this->window == nullptr) {
        SDL_DestroyProperties(props);
        panic("couldn't create window: " + std::string(SDL_GetError()));
    }
    SDL_DestroyProperties(props);

    // Set the refresh rate
    if (this->options->getFullscreen()) {
        for(SDL_DisplayMode * mode : getDisplayModes()) {
            if (mode->w == options->getScreenWidth() && mode->h == options->getScreenHeight()) {
                SDL_SetWindowFullscreenMode(window, mode);
                break;
            }
        }
    }

    this->renderer = SDL_CreateRenderer(this->window, nullptr);
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
    MIX_Quit();
    TTF_Quit();
    SDL_Quit();
}
