#include "ThemeManager.hpp"

#include "utils.hpp"

ThemeManager::ThemeManager(SDL_Renderer * renderer, Theme theme) {
    Mix_VolumeMusic(MIX_MAX_VOLUME/2);
    load(renderer, theme);
}

ThemeManager::~ThemeManager() {
    SDL_Log("ThemeManager destructor running");
    Mix_HaltMusic();
    Mix_FreeMusic(this->music);
    SDL_DestroyTexture(background);
}

void ThemeManager::load(SDL_Renderer * renderer, Theme theme) {
    switch (theme) {
        case Theme::MENU:
            this->music = NULL;
            this->background = loadBackground(renderer, "background1.jpg");
            break;
        case Theme::DEFAULT:
            this->music = Mix_LoadMUS("assets/music/song1.mp3");
            this->background = loadBackground(renderer, "background1.jpg");
        default:
            break;
    }
    if (this->music != NULL) {
        Mix_PlayMusic(music, -1);
    }
}

void ThemeManager::update() {

}

void ThemeManager::draw(SDL_Renderer * renderer) {
    SDL_RenderCopy(renderer, this->background, NULL, NULL);
}

SDL_Texture * ThemeManager::loadBackground(SDL_Renderer * renderer, std::string filename) {
    std::string path = "assets/backgrounds/" + filename;
    SDL_Surface *img = IMG_Load(path.c_str());

    if (img == nullptr) {
        panic("couldn't load image: " + std::string(IMG_GetError()));
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, img);
    SDL_FreeSurface(img);

    if (texture == nullptr) {
        panic("couldn't create texture from surface: " + std::string(SDL_GetError()));
    }

    return texture;
}