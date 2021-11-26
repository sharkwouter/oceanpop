#ifndef FONTMANAGER_HPP
#define FONTMANAGER_HPP

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

class FontManager {
private:
    TTF_Font * font_title;
    TTF_Font * font;
public:
    FontManager();
    ~FontManager();

    void load();
    SDL_Texture * getTexture(SDL_Renderer *renderer, std::string text, bool title, SDL_Color color);
};

#endif // FONTMANAGER_HPP
