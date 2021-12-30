#ifndef SHELL_HPP
#define SHELL_HPP

enum class ShellType {
    CRONCH,
    SHALLOP,
    CONE,
    CORAL,
    WEED,
    URCHIN,
    BUBBLE,
    NUMBER_OF_COLORS,
    NONE
};

struct Shell {
    ShellType type;
    int x;
    int y;
};

enum class Direction {
    HORIZONTAL,
    VERTICAL
};

struct Match {
    ShellType type;
    int x;
    int y;
    Direction direction;
};

#endif // SHELL_HPP
