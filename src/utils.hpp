#ifndef UTILS_HPP
#define UTILS_HPP

#include <SDL3/SDL.h>

#include <vector>
#include <string>

#include "TranslationManager.hpp"

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

SDL_DisplayMode * getStandardDisplayMode();
std::vector<SDL_DisplayMode*> getDisplayModes();

extern TranslationManager translation_manager;
#define _(x) translation_manager.translate(x)

#endif // UTILS_HPP
