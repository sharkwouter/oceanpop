#include "utils.hpp"
#include <stdexcept>
#include <filesystem>

#include <SDL.h>

void panic(const std::string &reason) {
    throw std::runtime_error(reason.c_str());
}

std::string getResourcePath(std::string file) {
    std::string path = SDL_GetBasePath();

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