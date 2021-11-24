#ifndef FONTMANAGER_HPP
#define FONTMANAGER_HPP

#include <map>
#include <string>
#include <SDL.h>

class FontManager {
private:
    std::map<std::string, SDL_Texture*> textures;
public:
    FontManager();
    ~FontManager();

    void add_font(const std::string &file, SDL_Renderer *renderer);
    SDL_Texture *get(const std::string &file);
};

#endif // FONTMANAGER_HPP
