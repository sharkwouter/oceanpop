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

static std::string getAssetPath(std::string type, std::string file) {
    std::string path = "";
    #if defined(NXDK) || defined(WIN32)
        std::string separator = "\\";
    #else
        std::string separator = "/";
    #endif

    #ifdef NXDK
        path += "D:\\";
    #else
    char * base_path = SDL_GetBasePath();
    if (base_path) {
        path += base_path;
        SDL_free(base_path);
    }
    #endif

    path += "assets";
    path += separator;
    path += type;
    path += separator;
    path += file;

    return path;
}

SDL_DisplayMode getStandardDisplayMode();
std::vector<SDL_DisplayMode> getDisplayModes();

#endif // UTILS_HPP
