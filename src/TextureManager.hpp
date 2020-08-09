#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include <map>
#include <string>
#include <SDL.h>

class TextureManager {
private:
    std::map<std::string, SDL_Texture*> textures;
public:
    TextureManager();
    ~TextureManager();

    void add_texture(const std::string &file, SDL_Renderer *renderer);
    SDL_Texture *get(const std::string &file);
};

#endif // TEXTUREMANAGER_HPP
