#ifndef BOARD_HPP
#define BOARD_HPP

class Board {
public:
    Board(int x, int y, int width, int height);
    void update();
    void draw();
    ~Board();
};

#endif // BOARD_HPP