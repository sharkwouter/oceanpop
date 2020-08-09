#include "utils.hpp"
#include <stdexcept>

void panic(const std::string &reason) {
    throw std::runtime_error(reason.c_str());
}
