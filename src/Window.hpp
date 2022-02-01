#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL.h>
#include <string>

#include "OptionManager.hpp"

class Window {
public:
    SDL_Window *window;
    SDL_Renderer *renderer;

    Window(const std::string &title, OptionManager * options);
    ~Window();

    unsigned int start_frame;
    float frame_delay;

    void clear();
    void present();
};

#endif // WINDOW_HPP
