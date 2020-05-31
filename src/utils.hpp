#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <stdexcept>

inline void panic(const std::string &reason) {
    throw std::runtime_error(reason.c_str());
}

inline void todo() {
    panic("not yet implemented");
}

#endif // UTILS_HPP
