#ifndef SHELL_HPP
#define SHELL_HPP

enum class ShellType {
    CRONCH,
    SHALLOP,
    CONE,
    CORAL,
    URCHIN,
    BUBBLE,
    NUMBER_OF_COLORS,
    NONE
};

struct Shell {
    int x;
    int y;
    ShellType type;
};

#endif // SHELL_HPP
