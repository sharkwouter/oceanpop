#include "FontManager.hpp"

#include "constants.hpp"
#include "utils.hpp"

FontManager::FontManager() {
    load();
}

FontManager::~FontManager() {
    TTF_CloseFont(font);
    TTF_CloseFont(font_title);
}

void FontManager::load() {
    font = TTF_OpenFont(getResourcePath("assets/fonts/2K4sRegular.ttf").c_str(), FONT_SIZE);
    font_title = TTF_OpenFont(getResourcePath("assets/fonts/2K4sRegular.ttf").c_str(), FONT_SIZE_TITLE);
    if (font == NULL || font_title == NULL) {
        panic("Couldn't load font: " + std::string(TTF_GetError()));
    }
}

SDL_Texture * FontManager::getTexture(SDL_Renderer *renderer, std::string text, bool title, SDL_Color color={0,0,0,255}) {
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
