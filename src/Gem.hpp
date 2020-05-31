#ifndef GEM_HPP
#define GEM_HPP

enum class GemType {
    NORMAL
};

class Gem {
public:
    Gem(int x, int y, GemType type);
    void update();
    void draw();
    void set_position(int x, int y);
    ~Gem();
};

#endif // GEM_HPP