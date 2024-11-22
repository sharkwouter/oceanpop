#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

inline constexpr float ANALOG_DEADZONE_MULTIPLIER = 0.5;

inline constexpr int DROP_TIME = 300; // in ms

inline constexpr int MATCH_TIME = 500; // in ms
inline constexpr int MATCH_STEPS = 100;
inline constexpr float MATCH_DELAY = MATCH_TIME/MATCH_STEPS;

inline constexpr int MIN_SHELL_SIZE = 16;
inline constexpr int MIN_SCREEN_WIDTH = MIN_SHELL_SIZE * 8;
inline constexpr int MIN_SCREEN_HEIGHT = MIN_SHELL_SIZE * 8;

// Everything below this are defaults
inline constexpr int DEFAULT_BOARD_WIDTH = 8;
inline constexpr int DEFAULT_BOARD_HEIGHT = 7;
inline constexpr int DEFAULT_SHELL_SIZE = 256; // The default should be the largest

#if !defined(__PSP__) && !defined(__vita__)
#   define SHOW_RESOLUTION_OPTIONS 1
#endif

#endif // CONSTANTS_HPP
