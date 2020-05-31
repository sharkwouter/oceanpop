#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL.h>
#include <string>

class Window
{
private:
    SDL_Window *window;
public:
    Window(const std::string &title, int width, int height);
    ~Window();
};

#endif // WINDOW_HPP
