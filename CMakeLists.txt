cmake_minimum_required(VERSION 3.10)

project(match-theory VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra -pedantic)

file(GLOB_RECURSE SRC src/*.cpp)
add_executable(match-theory ${SRC})

find_package(SDL2 REQUIRED)
target_include_directories(match-theory PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(match-theory ${SDL2_LIBRARIES})
