#ifndef GEM_HPP
#define GEM_HPP

#include <SDL.h>

class Gem {
private:
    SDL_Texture *texture;
    SDL_Rect src;
    int x;
    int y;
public:
    Gem(int dst_x, int dst_y, SDL_Texture *texture, SDL_Rect &src);

    void update();
    void draw(SDL_Renderer *renderer);
};

#endif // GEM_HPP
