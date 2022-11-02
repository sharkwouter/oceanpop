#include "FontManager.hpp"

#include "constants.hpp"
#include "utils.hpp"

FontManager::FontManager(OptionManager * options) : options(options) {
    load();
}

FontManager::~FontManager() {
    TTF_CloseFont(font);
    TTF_CloseFont(font_title);
}

void FontManager::load() {
    this->shellSize = this->options->getShellSize();
    font = TTF_OpenFont(getAssetPath("fonts", "2K4sRegular.ttf").c_str(), this->shellSize/2);
    font_title = TTF_OpenFont(getAssetPath("fonts", "2K4sRegular.ttf").c_str(), this->shellSize);
    if (font == NULL || font_title == NULL) {
        panic("Couldn't load font: " + std::string(TTF_GetError()));
    }
}

void FontManager::reload() {
    SDL_Log("Reloading fonts");
    TTF_CloseFont(font);
    TTF_CloseFont(font_title);
    load();
}

SDL_Texture * FontManager::getTexture(SDL_Renderer *renderer, std::string text, bool title, SDL_Color color={0,0,0,255}) {
    if (this->shellSize != this->options->getShellSize()) {
        reload();
    }
    TTF_Font * current_font = this->font;
    if (title) {
        current_font = this->font_title;
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
