#ifndef FONTMANAGER_HPP
#define FONTMANAGER_HPP

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

#include "FontType.hpp"

#include "OptionManager.hpp"

class FontManager {
private:
    OptionManager * options;

    TTF_Font * font_title;
    TTF_Font * font;
    TTF_Font * font_small;

    int shellSize;

    void load();
    void reload();
public:
    FontManager(OptionManager * options);
    ~FontManager();

    SDL_Texture * getTexture(SDL_Renderer *renderer, std::string text, FontType font_type, SDL_Color color);
};

#endif // FONTMANAGER_HPP
