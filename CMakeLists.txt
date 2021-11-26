cmake_minimum_required(VERSION 3.10)

project(match-theory VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
     add_compile_options(-Wall -Wextra -pedantic)
endif()

file(GLOB_RECURSE SRC src/*.cpp)
add_executable(match-theory ${SRC})

include(FindPkgConfig)

pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image)

target_include_directories(match-theory PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
target_link_libraries(match-theory SDL2_image ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/images
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})