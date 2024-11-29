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

enum class MatchType {
    HORIZONTAL,
    VERTICAL,
    BONUS // Having a shell in both a vertical and horizontal match results in a bonus point
};

struct Match {
    ShellType type;
    int x;
    int y;
    MatchType match_type;
};

#endif // SHELL_HPP
