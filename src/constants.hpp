#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

inline constexpr int DROP_TIMER = 250;

#if defined(__vita__)
inline constexpr int SCREEN_WIDTH = 960;
inline constexpr int SCREEN_HEIGHT = 544;
inline constexpr int GEM_SIZE = 64;
#elif defined(__PSP__)
inline constexpr int SCREEN_WIDTH = 480;
inline constexpr int SCREEN_HEIGHT = 272;
inline constexpr int GEM_SIZE = 32;
#else
inline constexpr int SCREEN_WIDTH = 769;
inline constexpr int SCREEN_HEIGHT = 673;
inline constexpr int GEM_SIZE = 96;
#endif

#endif // CONSTANTS_HPP
