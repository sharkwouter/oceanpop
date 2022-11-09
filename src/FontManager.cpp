#include "FontManager.hpp"

#include "constants.hpp"
#include "utils.hpp"

FontManager::FontManager(OptionManager * options) : options(options) {
    load();
}

FontManager::~FontManager() {
    TTF_CloseFont(font_title);
    TTF_CloseFont(font);
    TTF_CloseFont(font_small);
}

void FontManager::load() {
    this->shellSize = this->options->getShellSize();
    font_small = TTF_OpenFont(getResourcePath("assets/fonts/2K4sRegular.ttf").c_str(), this->shellSize/4);
    font = TTF_OpenFont(getResourcePath("assets/fonts/2K4sRegular.ttf").c_str(), this->shellSize/2);
    font_title = TTF_OpenFont(getResourcePath("assets/fonts/2K4sRegular.ttf").c_str(), this->shellSize);
    if (font_small == NULL || font == NULL || font_title == NULL) {
        panic("Couldn't load font: " + std::string(TTF_GetError()));
    }
}

void FontManager::reload() {
    SDL_Log("Reloading fonts");
    TTF_CloseFont(font_title);
    TTF_CloseFont(font);
    TTF_CloseFont(font_small);
    load();
}

SDL_Texture * FontManager::getTexture(SDL_Renderer *renderer, std::string text, FontType font_type, SDL_Color color={0,0,0,255}) {
    if (this->shellSize != this->options->getShellSize()) {
        reload();
    }

    TTF_Font * current_font;
    switch (font_type)
    {
    case FontType::NORMAL:
        current_font = this->font;
        break;
    case FontType::TITLE:
        current_font = this->font_title;
        break;
    case FontType::SMALL:
        current_font = this->font_small;
        break;
    }
    
    SDL_Surface * surface = TTF_RenderUTF8_Blended(current_font, text.c_str(), color);
    if (surface == NULL) {
        SDL_Log("Couldn't create surface for text %s: %s", text.c_str(), TTF_GetError());
        return NULL;
    }

    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}
