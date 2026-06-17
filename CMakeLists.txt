cmake_minimum_required(VERSION 3.22)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
  message(FATAL_ERROR "Please create a build directory. Building inside the oceanpop directory directly would prevent the asset directory from being build correctly.")
endif()

project(oceanpop)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -pg -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

file(GLOB_RECURSE SRC src/*.cpp)

if(WIN32)
    add_executable(${CMAKE_PROJECT_NAME} WIN32 ${SRC} ${CMAKE_CURRENT_SOURCE_DIR}/platform/windows/appicon.rc)
else()
    add_executable(${CMAKE_PROJECT_NAME} ${SRC})
endif()

set(BUILD_STATIC_DEFAULT OFF)
if(WIN32 OR VITA OR PSP OR PS2)
    set(BUILD_STATIC_DEFAULT ON)
endif()
option(BUILD_STATIC "Build a static binary" ${BUILD_STATIC_DEFAULT})


# Link libraries
if(BUILD_STATIC)
    set(CMAKE_CXX_FLAGS "-static ${CMAKE_CXX_FLAGS}")
endif()

# Build libraries
set(BUILD_STATIC_LIBS ON)
set(BUILD_SHARED_LIBS OFF)
set(BUILD_OBJECT_LIBS OFF)

# SDL
if(CMAKE_SYSTEM_NAME MATCHES "NintendoWii")
    include(FindPkgConfig)
    pkg_search_module(SDL3 REQUIRED sdl3)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${SDL3_STATIC_LIBRARIES}
    )
else()
    set(SDL_SHARED OFF CACHE INTERNAL "" FORCE)
    set(SDL_STATIC ON CACHE INTERNAL "" FORCE)
    set(SDL_DISABLE_INSTALL ON CACHE INTERNAL "" FORCE)
    set(SDL_DISABLE_INSTALL_DOCS ON CACHE INTERNAL "" FORCE)
    set(SDL_TEST_LIBRARY OFF CACHE INTERNAL "" FORCE)
    set(SDL_TESTS OFF CACHE INTERNAL "" FORCE)
    set(SDL_EXAMPLES OFF CACHE INTERNAL "" FORCE)
    add_subdirectory(vendor/SDL)

    target_link_libraries(${PROJECT_NAME} PRIVATE
        SDL3::SDL3-static
    )
endif()


# jsoncpp
set(JSONCPP_WITH_TESTS OFF)
set(JSONCPP_WITH_POST_BUILD_UNITTEST OFF)
set(JSONCPP_WITH_PKGCONFIG_SUPPORT OFF)
set(JSONCPP_WITH_CMAKE_PACKAGE OFF)
add_subdirectory(vendor/jsoncpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
    jsoncpp_static
)
include_directories(
    vendor/jsoncpp/include/json
)

# SDL_image
set(SDLIMAGE_INSTALL OFF CACHE INTERNAL "" FORCE)
set(SDLIMAGE_SAMPLES OFF CACHE INTERNAL "" FORCE)
set(SDLIMAGE_TESTS OFF CACHE INTERNAL "" FORCE)
set(SDLIMAGE_ANI OFF CACHE INTERNAL "" FORCE)
set(SDLIMAGE_AVIF OFF CACHE INTERNAL "" FORCE)
set(SDLIMAGE_BMP ON CACHE INTERNAL "" FORCE)
set(SDLIMAGE_GIF OFF CACHE INTERNAL "" FORCE)
set(SDLIMAGE_JPG ON CACHE INTERNAL "" FORCE)
set(SDLIMAGE_JPG_SHARED OFF CACHE INTERNAL "" FORCE)
set(SDLIMAGE_JPG_SAVE OFF CACHE INTERNAL "" FORCE)
set(SDLIMAGE_JXL OFF CACHE INTERNAL "" FORCE)
set(SDLIMAGE_LBM OFF CACHE INTERNAL "" FORCE)
set(SDLIMAGE_PCX OFF CACHE INTERNAL "" FORCE)
set(SDLIMAGE_SVG ON CACHE INTERNAL "" FORCE)
set(SDLIMAGE_PNG ON CACHE INTERNAL "" FORCE)
set(SDLIMAGE_PNG_SAVE OFF CACHE INTERNAL "" FORCE)
set(SDLIMAGE_PNG_LIBPNG OFF CACHE INTERNAL "" FORCE)
set(SDLIMAGE_PNG_SHARED OFF CACHE INTERNAL "" FORCE)
set(SDLIMAGE_PNM OFF CACHE INTERNAL "" FORCE)
set(SDLIMAGE_QOI OFF CACHE INTERNAL "" FORCE)
set(SDLIMAGE_TGA OFF CACHE INTERNAL "" FORCE)
set(SDLIMAGE_TIF OFF CACHE INTERNAL "" FORCE)
set(SDLIMAGE_WEBP OFF CACHE INTERNAL "" FORCE)
set(SDLIMAGE_XCF OFF CACHE INTERNAL "" FORCE)
set(SDLIMAGE_XPM OFF CACHE INTERNAL "" FORCE)
set(SDLIMAGE_XV OFF CACHE INTERNAL "" FORCE)
set(SDLIMAGE_DEPS_SHARED OFF CACHE INTERNAL "" FORCE)
set(SDLIMAGE_VENDORED ON CACHE INTERNAL "" FORCE)
set(SDLIMAGE_BACKEND_IMAGEIO OFF CACHE INTERNAL "" FORCE)
set(SDLIMAGE_BACKEND_STB ON CACHE INTERNAL "" FORCE)
set(CMAKE_POSITION_INDEPENDENT_CODE OFF)
add_subdirectory(vendor/SDL_image)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3_image::SDL3_image-static
)
include_directories(
    vendor/SDL_image/include/
)

#SDL_mixer
set(SDLMIXER_INSTALL OFF CACHE INTERNAL "" FORCE)
set(SDLMIXER_DEPS_SHARED OFF CACHE INTERNAL "" FORCE)
set(SDLMIXER_VENDORED ON CACHE INTERNAL "" FORCE)
set(SDLMIXER_TESTS OFF CACHE INTERNAL "" FORCE)
set(SDLMIXER_EXAMPLES OFF CACHE INTERNAL "" FORCE)
set(SDLMIXER_AIFF OFF CACHE INTERNAL "" FORCE)
set(SDLMIXER_AU OFF CACHE INTERNAL "" FORCE)
set(SDLMIXER_VOC OFF CACHE INTERNAL "" FORCE)
set(SDLMIXER_FLAC OFF CACHE INTERNAL "" FORCE)
set(SDLMIXER_GME OFF CACHE INTERNAL "" FORCE)
set(SDLMIXER_OPUS OFF CACHE INTERNAL "" FORCE)
set(SDLMIXER_VORBIS_STB OFF CACHE INTERNAL "" FORCE)
set(SDLMIXER_VORBIS_VORBISFILE OFF CACHE INTERNAL "" FORCE)
set(SDLMIXER_VORBIS_TREMOR OFF CACHE INTERNAL "" FORCE)
set(SDLMIXER_MOD OFF CACHE INTERNAL "" FORCE)
set(SDLMIXER_WAVPACK OFF CACHE INTERNAL "" FORCE)
set(SDLMIXER_MIDI OFF CACHE INTERNAL "" FORCE)
set(SDLMIXER_MP3 ON CACHE INTERNAL "" FORCE)
set(SDLMIXER_MP3_DRMP3 ON CACHE INTERNAL "" FORCE)
set(SDLMIXER_MP3_MPG123 OFF CACHE INTERNAL "" FORCE)
set(SDLMIXER_MP3_MPG123_SHARED OFF CACHE INTERNAL "" FORCE)
set(SDLMIXER_WAVE ON CACHE INTERNAL "" FORCE)
add_subdirectory(vendor/SDL_mixer)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3_mixer::SDL3_mixer-static
)
include_directories(
    vendor/SDL_mixer/include/
)

# SDL_ttf
set(SDLTTF_INSTALL OFF CACHE INTERNAL "" FORCE)
set(SDLTTF_VENDORED ON CACHE INTERNAL "" FORCE)
set(SDLTTF_SAMPLES OFF CACHE INTERNAL "" FORCE)
set(SDLTTF_FREETYPE ON CACHE INTERNAL "" FORCE)
set(SDLTTF_HARFBUZZ OFF CACHE INTERNAL "" FORCE)
set(SDLTTF_PLUTOSVG OFF CACHE INTERNAL "" FORCE)
add_subdirectory(vendor/SDL_ttf)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3_ttf::SDL3_ttf-static
)
include_directories(
    vendor/SDL_ttf/include/
)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

# Set asset paths
set(ASSETS_FONTS "${CMAKE_SOURCE_DIR}/assets/fonts")
set(ASSETS_IMAGES "${CMAKE_SOURCE_DIR}/assets/images")
set(ASSETS_LEVELS "${CMAKE_SOURCE_DIR}/assets/levels")
set(ASSETS_SOUNDS "${CMAKE_SOURCE_DIR}/assets/sounds")
set(ASSETS_MUSIC "${CMAKE_SOURCE_DIR}/assets/music")
set(ASSETS_BACKGROUNDS "${CMAKE_SOURCE_DIR}/assets/backgrounds")

if(PSP)
    set(ASSETS_BACKGROUNDS "${CMAKE_CURRENT_SOURCE_DIR}/platform/psp/assets/backgrounds")
elseif(PS2)
    set(ASSETS_BACKGROUNDS "${CMAKE_CURRENT_SOURCE_DIR}/platform/ps2/assets/backgrounds")
    set(ASSETS_MUSIC "${CMAKE_CURRENT_SOURCE_DIR}/platform/ps2/assets/music")    
elseif(VITA)
    set(ASSETS_BACKGROUNDS "${CMAKE_SOURCE_DIR}/platform/vita/assets/backgrounds")
elseif(CMAKE_SYSTEM_NAME MATCHES "NintendoWii")
    set(ASSETS_BACKGROUNDS "${CMAKE_SOURCE_DIR}/platform/wii/assets/backgrounds")
endif()

# Copy assets
if(NOT VITA)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/assets)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ASSETS_BACKGROUNDS} ${CMAKE_CURRENT_BINARY_DIR}/assets/backgrounds)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ASSETS_FONTS} ${CMAKE_CURRENT_BINARY_DIR}/assets/fonts)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ASSETS_IMAGES} ${CMAKE_CURRENT_BINARY_DIR}/assets/images)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ASSETS_LEVELS} ${CMAKE_CURRENT_BINARY_DIR}/assets/levels)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ASSETS_MUSIC} ${CMAKE_CURRENT_BINARY_DIR}/assets/music)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ASSETS_SOUNDS} ${CMAKE_CURRENT_BINARY_DIR}/assets/sounds)
endif()

# Generate credits file
set(CREDITS_FILE "${CMAKE_CURRENT_BINARY_DIR}/assets/CREDITS.md")

file(READ "${CMAKE_SOURCE_DIR}/assets/CREDITS_header.md" CONTENTS)
file(WRITE "${CREDITS_FILE}" "${CONTENTS}")

if(BUILD_STATIC)
    file(READ "${CMAKE_SOURCE_DIR}/assets/CREDITS_static.md" CONTENTS)
    file(APPEND "${CREDITS_FILE}" "${CONTENTS}")
endif()
if(PSP)
    file(READ "${CMAKE_SOURCE_DIR}/assets/CREDITS_PSP.md" CONTENTS)
    file(APPEND "${CREDITS_FILE}" "${CONTENTS}")
elseif(VITA)
    file(READ "${CMAKE_SOURCE_DIR}/assets/CREDITS_Vita.md" CONTENTS)
    file(APPEND "${CREDITS_FILE}" "${CONTENTS}")
elseif(PS2)
    file(READ "${CMAKE_SOURCE_DIR}/assets/CREDITS_PS2.md" CONTENTS)
    file(APPEND "${CREDITS_FILE}" "${CONTENTS}")
elseif(CMAKE_SYSTEM_NAME MATCHES "NintendoWii")
    file(READ "${CMAKE_SOURCE_DIR}/assets/CREDITS_Wii.md" CONTENTS)
    file(APPEND "${CREDITS_FILE}" "${CONTENTS}")
endif()

file(READ "${CMAKE_SOURCE_DIR}/assets/CREDITS_footer.md" CONTENTS)
file(APPEND "${CREDITS_FILE}" "${CONTENTS}")

# Platform specific install
if (VITA)
    include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)
    vita_create_self(${CMAKE_PROJECT_NAME}.self ${CMAKE_PROJECT_NAME})

    vita_create_vpk(${CMAKE_PROJECT_NAME}.vpk "OPOP00001" ${CMAKE_PROJECT_NAME}.self
        VERSION "01.00"
        NAME OceanPop
        FILE platform/vita/sce_sys sce_sys
        FILE "${CREDITS_FILE}" assets/CREDITS.md
        FILE "${ASSETS_BACKGROUNDS}" assets/backgrounds
        FILE "${ASSETS_FONTS}" assets/fonts
        FILE "${ASSETS_IMAGES}" assets/images
        FILE "${ASSETS_LEVELS}" assets/levels
        FILE "${ASSETS_MUSIC}" assets/music
        FILE "${ASSETS_SOUNDS}" assets/sounds
        FILE "${ASSETS_LANGUAGES}" assets/languages
    )
elseif(PS2)
	set(CMAKE_EXECUTABLE_SUFFIX .elf)
elseif(PLATFORM_PSP)
    create_pbp_file(
        TARGET ${CMAKE_PROJECT_NAME}
        ICON_PATH ${CMAKE_SOURCE_DIR}/platform/psp/icon.png
        BACKGROUND_PATH NULL
        PREVIEW_PATH NULL
        TITLE OceanPop
    )
elseif(CMAKE_SYSTEM_NAME MATCHES "NintendoWii")
    ogc_create_dol(oceanpop)
elseif(win32)
    # Nothing here yet
else()
    install(TARGETS ${PROJECT_NAME} DESTINATION bin)
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/assets DESTINATION share/${PROJECT_NAME})
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/platform/unix/io.github.sharkwouter.oceanpop.desktop DESTINATION share/applications)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/platform/unix/io.github.sharkwouter.oceanpop.metainfo.xml DESTINATION share/metainfo)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/platform/unix/icons/128x128/io.github.sharkwouter.oceanpop.png DESTINATION share/icons/hicolor/128x128/apps)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/README.md DESTINATION share/doc/${PROJECT_NAME})
endif()
