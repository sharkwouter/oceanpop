#include "ThemeManager.hpp"

#include "utils.hpp"

ThemeManager::ThemeManager(SDL_Renderer * renderer, Theme theme) : renderer(renderer){
    this->theme = theme;
    Mix_VolumeMusic(MIX_MAX_VOLUME/2);
    load();
}

ThemeManager::~ThemeManager() {
    Mix_HaltMusic();
    Mix_FreeMusic(this->music);
    SDL_DestroyTexture(this->background);
}

void ThemeManager::load() {
    switch (this->theme) {
        case Theme::MENU:
            this->music = NULL;
            this->background = loadBackground("background1.jpg");
            break;
        case Theme::THEME1:
            this->music = Mix_LoadMUS("assets/music/song1.mp3");
            this->background = loadBackground("background1.jpg");
            break;
        case Theme::THEME2:
            this->music = Mix_LoadMUS("assets/music/song2.mp3");
            this->background = loadBackground("background1.jpg");
            break;
        case Theme::THEME3:
            this->music = Mix_LoadMUS("assets/music/song3.mp3");
            this->background = loadBackground("background1.jpg");
            break;
        case Theme::THEME4:
            this->music = Mix_LoadMUS("assets/music/song4.mp3");
            this->background = loadBackground("background1.jpg");
            break;
        default:
            this->theme = Theme::THEME1;
            load();
            break;
    }
    if (this->music != NULL) {
        Mix_PlayMusic(this->music, 0);
    }
}

void ThemeManager::update() {
    if (this->music != NULL && !Mix_PlayingMusic()) {
        next();
    }

}

void ThemeManager::draw(SDL_Renderer * renderer) {
    SDL_RenderCopy(renderer, this->background, NULL, NULL);
}

void ThemeManager::next() {
    this->theme = (Theme) (((int) this->theme + 1) % (int) Theme::AMOUNT);
    if (this->theme == Theme::MENU) {
        this->theme = Theme::THEME1;
    }
    Mix_HaltMusic();
    Mix_FreeMusic(this->music);
    this->music = NULL;
    SDL_DestroyTexture(background);
    this->background = NULL;
    load();
}

SDL_Texture * ThemeManager::loadBackground(std::string filename) {
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