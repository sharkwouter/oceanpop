#include "utils.hpp"
#include <stdexcept>
#include <filesystem>

#include <SDL.h>

void panic(const std::string &reason) {
    throw std::runtime_error(reason.c_str());
}

std::string getResourcePath(std::string file) {
    std::string path = "";

    char * base_path = SDL_GetBasePath();
    if (base_path) {
        path += base_path;
        SDL_free(base_path);
    }

    #if !defined(WIN32) && !defined(__PSP__) && !defined(__vita__)
        std::string share = path + "../share/oceanpop/";
        std::filesystem::path share_path{share};

        if (std::filesystem::exists(share_path)) {
            path = share;
        }
    #endif

    path += file;

    return path;
}