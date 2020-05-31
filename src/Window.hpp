#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL.h>
#include <string>

class Window {
public:
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool should_close;

    Window(const std::string &title, int width, int height);
    void handle_input();
    void clear();
    void present();
    ~Window();
};

#endif // WINDOW_HPP
