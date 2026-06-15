#include "utils.hpp"

#include <stdexcept>
#include <filesystem>

#include "constants.hpp"

void panic(const std::string &reason) {
    throw std::runtime_error(reason.c_str());
}

std::string getResourcePath(std::string file) {
    std::string path = "";

#if defined(__wii__)
    const char * base_path = SDL_GetPrefPath(NULL,"oceanpop");
#else
   const  char * base_path = SDL_GetBasePath();
#endif
    if (base_path) {
        path += base_path;
    }

    #if !defined(WIN32) && !defined(SDL_PLATFORM_PSP) && !defined(__vita__) && !defined(SDL_PLATFORM_PS2)
        std::string share = path + "../share/oceanpop/";
        std::filesystem::path share_path{share};

        if (std::filesystem::exists(share_path)) {
            path = share;
        }
    #endif

    path += file;

    return path;
}

SDL_DisplayMode * getStandardDisplayMode() {
    SDL_DisplayID display = SDL_GetPrimaryDisplay();
    return const_cast<SDL_DisplayMode *>(SDL_GetCurrentDisplayMode(display));
}

std::vector<SDL_DisplayMode*> getDisplayModes() {
    std::vector<SDL_DisplayMode*> result;
    SDL_DisplayID display = SDL_GetPrimaryDisplay();
    int display_mode_count = 0;
    SDL_DisplayMode ** modes = SDL_GetFullscreenDisplayModes(display, &display_mode_count);
    if (display_mode_count > 0) {
        for (int i = 0; i < display_mode_count; ++i) {
            SDL_DisplayMode * mode = modes[i];
            result.push_back(mode);
        }
    }

    return result;
}
