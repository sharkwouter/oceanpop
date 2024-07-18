#ifndef UTILS_HPP
#define UTILS_HPP

#include <SDL.h>

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

SDL_DisplayMode getStandardDisplayMode();
std::vector<SDL_DisplayMode> getDisplayModes();

#ifdef TRANSLATION_SUPPORT
    extern TranslationManager translation_manager;
    #define _(x) translation_manager.translate(x)
#else
    #define _(x) x
#endif

#endif // UTILS_HPP
