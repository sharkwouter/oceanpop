#ifndef UTILS_HPP
#define UTILS_HPP

#include <SDL.h>

#include <vector>
#include <string>

struct DisplayMode
{
    int width;
    int height;
};


void panic(const std::string &reason);

inline void todo() {
    panic("not yet implemented");
}

std::string getResourcePath(std::string file);

std::vector<SDL_DisplayMode> getDisplayModes();

#endif // UTILS_HPP
