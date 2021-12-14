#include "ThemeManager.hpp"

#include "utils.hpp"

ThemeManager::ThemeManager(SDL_Renderer * renderer, Theme theme) : renderer(renderer){
    Mix_VolumeMusic(MIX_MAX_VOLUME/2);

    this->theme = theme;
    load(this->theme);
}

ThemeManager::~ThemeManager() {
    Mix_HaltMusic();
    Mix_FreeMusic(this->music);
    SDL_DestroyTexture(this->background);
}

void ThemeManager::load(Theme theme) {
    loadBackground(theme);
    loadMusic(theme);
}

void ThemeManager::loadBackground(Theme theme) {
    switch (theme) {
        case Theme::THEME1:
            this->background = createBackgroundTexture("background1.jpg");
            break;
        case Theme::THEME2:
            this->background = createBackgroundTexture("background2.jpg");
            break;
        case Theme::THEME3:
            this->background = createBackgroundTexture("background3.jpg");
            break;
        case Theme::THEME4:
            this->background = createBackgroundTexture("background4.jpg");
            break;
        default:
            this->background = createBackgroundTexture("menu.jpg");
            break;
    }
}

void ThemeManager::loadMusic(Theme theme) {
    switch (theme) {
        case Theme::THEME1:
            this->music = Mix_LoadMUS("assets/music/song1.mp3");
            break;
        case Theme::THEME2:
            this->music = Mix_LoadMUS("assets/music/song2.mp3");
            break;
        case Theme::THEME3:
            this->music = Mix_LoadMUS("assets/music/song3.mp3");
            break;
        case Theme::THEME4:
            this->music = Mix_LoadMUS("assets/music/song4.mp3");
            break;
        default:
            this->music = NULL;
            break;
    }
    if (this->music != NULL) {
        Mix_PlayMusic(this->music, 0);
    }
}

void ThemeManager::update() {
    if (this->music != NULL && !this->paused && !Mix_PlayingMusic()) {
        nextSong();
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
    load(this->theme);
}

void ThemeManager::nextSong() {
    this->theme = (Theme) (((int) this->theme + 1) % (int) Theme::AMOUNT);
    if (this->theme == Theme::MENU) {
        this->theme = Theme::THEME1;
    }
    Mix_HaltMusic();
    Mix_FreeMusic(this->music);
    this->music = NULL;
    loadMusic(this->theme);
}

void ThemeManager::switchTheme(int theme) {
    if (theme < 1 || theme >= (int) Theme::AMOUNT) {
        theme = 1;
    }

    this->theme = (Theme) theme;

    Mix_HaltMusic();
    Mix_FreeMusic(this->music);
    this->music = NULL;
    SDL_DestroyTexture(background);
    this->background = NULL;

    load(this->theme);
}

void ThemeManager::pause() {
    this->paused = true;
    Mix_HaltMusic();
}

void ThemeManager::unpause() {
    this->paused = false;
    Mix_PlayMusic(this->music, 0);
}

SDL_Texture * ThemeManager::createBackgroundTexture(std::string filename) {
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