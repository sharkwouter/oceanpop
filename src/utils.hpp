#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

void panic(const std::string &reason);

inline void todo() {
    panic("not yet implemented");
}

#endif // UTILS_HPP
