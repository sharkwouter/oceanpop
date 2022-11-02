#include "utils.hpp"

#include <stdexcept>
#include <filesystem>

#include "constants.hpp"

void panic(const std::string &reason) {
    // throw std::runtime_error(reason.c_str());
}

std::string getResourcePath(std::string file) {
    std::string path = "";

    char * base_path = SDL_GetBasePath();
    if (base_path) {
        path += base_path;
        SDL_free(base_path);
    }

    #if !defined(WIN32) && !defined(__PSP__) && !defined(__vita__) && !defined(NXDK)
        std::string share = path + "../share/oceanpop/";
        std::filesystem::path share_path{share};

        if (std::filesystem::exists(share_path)) {
            path = share;
        }
    #endif

    path += file;

    return path;
}

SDL_DisplayMode getStandardDisplayMode() {
    SDL_DisplayMode mode;
    int display = 0;
    if (SDL_GetDesktopDisplayMode(display, &mode) != 0) {
        mode = getDisplayModes()[0];
    }
    return mode;
}

std::vector<SDL_DisplayMode> getDisplayModes() {
    std::vector<SDL_DisplayMode> result;
    int display = 0;
    int display_mode_count = SDL_GetNumDisplayModes(display);
    if (display_mode_count > 0) {
        for (int i = 0; i < display_mode_count; ++i) {
            SDL_DisplayMode mode;
            if (SDL_GetDisplayMode(display, i, &mode) != 0) {
                break;
            }
            result.push_back(mode);
        }
    }

    if (result.empty()) {
        SDL_Log("Display mode list is empty, using semi safe standard values");
        result.push_back({SDL_PIXELFORMAT_RGBA8888, 800, 600, 0, 0});
    }

    return result;
}
