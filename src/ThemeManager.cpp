#include "ThemeManager.hpp"

#include "utils.hpp"

ThemeManager::ThemeManager(SDL_Renderer * renderer, int theme, bool repeat) {
    loadBackground(renderer, theme);

    this->repeat = 0;
    if (repeat) {
        this->repeat = -1;
    }

    Mix_VolumeMusic(MIX_MAX_VOLUME/2);
    this->next_track = theme;
}

ThemeManager::~ThemeManager() {
    Mix_HaltMusic();
    Mix_FreeMusic(this->music);
    SDL_DestroyTexture(background);
}

void ThemeManager::loadBackground(SDL_Renderer * renderer, int theme) {
    switch (theme)
    {
        case 1:
            background = loadTexture(renderer, "assets/backgrounds/background1.jpg");
            break;
        default:
            loadBackground(renderer, 1);
            break;
    }
}

void ThemeManager::loadMusic() {
    switch (this->next_track)
    {
    case 1:
        music = Mix_LoadMUS("assets/music/song1.mp3");
        break;
    default:
        this->next_track = 1;
        loadMusic();
        return;
        break;
    }
    this->next_track++;
}

void ThemeManager::update() {
    if (!Mix_PlayingMusic()) {
        loadMusic();
        Mix_PlayMusic(this->music, this->repeat);
    }
}

void ThemeManager::draw(SDL_Renderer * renderer) {
    SDL_RenderCopy(renderer, this->background, NULL, NULL);
}

SDL_Texture * ThemeManager::loadTexture(SDL_Renderer * renderer, std::string path) {
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