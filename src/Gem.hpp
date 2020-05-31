#ifndef GEM_HPP
#define GEM_HPP

#include <SDL.h>

enum class GemType {
    RED,
    GREEN,
    BLUE
};

class Gem {
private:
    SDL_Texture *texture;
    SDL_Rect rect;
public:
    Gem(int x, int y, GemType type, SDL_Renderer *renderer);
    ~Gem();

    void update();
    void draw(SDL_Renderer *renderer);
    void set_position(int x, int y);
};

#endif // GEM_HPP