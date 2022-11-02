#include <SDL_image.h>
#include <SDL_ttf.h>
#ifdef NXDK
    #include <hal/video.h>
#else
    #include <SDL_mixer.h>
#endif
#include "Window.hpp"
#include "utils.hpp"

Window::Window(const std::string &title, OptionManager * options) {
    #ifdef NXDK
        XVideoSetMode(640, 480, 16, REFRESH_DEFAULT);
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

    #ifndef NXDK
    if( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1 ) {
		panic("couldn't init SDL_mixer: " + std::string(Mix_GetError()));
    }
    #endif

    // Set default screen resolution and refresh rate if they haven't been set yet
    if (options->getScreenRefreshRate() == 0) {
        SDL_DisplayMode mode = getStandardDisplayMode();
        options->setScreenResolution(mode.w, mode.h, mode.refresh_rate);
    }

    Uint32 window_flags = SDL_WINDOW_SHOWN;
    if (options->getFullscreen()) {
        window_flags |= SDL_WINDOW_FULLSCREEN;
    }
    this->window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, options->getScreenWidth(), options->getScreenHeight(), window_flags);
    if (this->window == nullptr) {
        panic("couldn't create window: " + std::string(SDL_GetError()));
    }

    // Set the refresh rate
    if (options->getFullscreen()) {
        for(SDL_DisplayMode mode : getDisplayModes()) {
            if (mode.w == options->getScreenWidth() && mode.h == options->getScreenHeight() && mode.refresh_rate == options->getScreenRefreshRate()) {
                SDL_SetWindowDisplayMode(window, &mode);
                break;
            }
        }
    }

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

Window::~Window() {
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    #ifndef NXDK
    Mix_Quit();
    Mix_CloseAudio();
    #endif
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
