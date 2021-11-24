#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

inline constexpr int DROP_TIMER = 250;

#ifndef __vita__
inline constexpr int SCREEN_WIDTH = 769;
inline constexpr int SCREEN_HEIGHT = 673;
inline constexpr int GEM_SIZE = 96;
#else
inline constexpr int SCREEN_WIDTH = 960;
inline constexpr int SCREEN_HEIGHT = 544;
inline constexpr int GEM_SIZE = 64;
#endif

#endif // CONSTANTS_HPP
