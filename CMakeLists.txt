cmake_minimum_required(VERSION 3.10)

project(match-theory VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra -pedantic)

file(GLOB_RECURSE SRC src/*.cpp)
add_executable(match-theory ${SRC})

include(FindPkgConfig)

find_package(SDL2 REQUIRED)
target_include_directories(match-theory PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(match-theory ${SDL2_LIBRARIES})

pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image >= 2.0.0)
target_include_directories(match-theory PRIVATE ${${SDL2_IMAGE_INCLUDE_DIRS}})
target_link_libraries(match-theory ${SDL2_IMAGE_LIBRARIES})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/images
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})