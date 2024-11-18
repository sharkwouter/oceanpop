cmake_minimum_required(VERSION 3.12)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
  message(FATAL_ERROR "Please create a build directory. Building inside the oceanpop directory directly would prevent the asset directory from being build correctly.")
endif()

project(oceanpop)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

file(GLOB_RECURSE SRC src/*.cpp)

if(WIN32)
    add_executable(${CMAKE_PROJECT_NAME} WIN32 ${SRC} ${CMAKE_CURRENT_SOURCE_DIR}/platform/windows/appicon.rc)
else()
    add_executable(${CMAKE_PROJECT_NAME} ${SRC})
endif()

set(BUILD_STATIC_DEFAULT OFF)
if(WIN32 OR VITA OR PSP)
    set(BUILD_STATIC_DEFAULT ON)
endif()
option(BUILD_STATIC "Build a static binary" ${BUILD_STATIC_DEFAULT})

# Find libraries
include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image)
pkg_search_module(SDL2_MIXER REQUIRED SDL2_mixer)
pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)
pkg_search_module(JSONCPP REQUIRED jsoncpp)
pkg_search_module(TINYGETTEXT QUIET tinygettext)

# Enable translation support by default if the tinygettext library is found
set(TRANSLATION_SUPPORT_DEFAULT OFF)
if(TINYGETTEXT_FOUND)
    message("-- Found tinygettext library")
    set(TRANSLATION_SUPPORT_DEFAULT ON)
endif()
option(TRANSLATION_SUPPORT "Add support for loading translations, requires tinygettext" ${TRANSLATION_SUPPORT_DEFAULT})

# Link libraries
if(BUILD_STATIC)
    set(CMAKE_CXX_FLAGS "-static ${CMAKE_CXX_FLAGS}")
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${SDL2_STATIC_LIBRARIES}
        ${SDL2_IMAGE_STATIC_LIBRARIES}
        ${SDL2_MIXER_STATIC_LIBRARIES}
        ${SDL2_TTF_STATIC_LIBRARIES}
        ${JSONCPP_STATIC_LIBRARIES}
    )
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_MIXER_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        ${JSONCPP_LIBRARIES}
    )
endif()

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${JSONCPP_INCLUDE_DIRS}
)

if(TRANSLATION_SUPPORT)
    message("-- Building with translation support")
    if(BUILD_STATIC)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${TINYGETTEXT_STATIC_LIBRARIES})
    else()
        target_link_libraries(${PROJECT_NAME} PRIVATE  ${TINYGETTEXT_LIBRARIES})
    endif()

    add_compile_definitions(TRANSLATION_SUPPORT=1)
else()
    message("-- Building without translation support")
endif()

# Set asset paths
set(ASSETS_FONTS "${CMAKE_SOURCE_DIR}/assets/fonts")
set(ASSETS_IMAGES "${CMAKE_SOURCE_DIR}/assets/images")
set(ASSETS_LEVELS "${CMAKE_SOURCE_DIR}/assets/levels")
set(ASSETS_SOUNDS "${CMAKE_SOURCE_DIR}/assets/sounds")
set(ASSETS_MUSIC "${CMAKE_SOURCE_DIR}/assets/music")
set(ASSETS_BACKGROUNDS "${CMAKE_SOURCE_DIR}/assets/backgrounds")
if(TRANSLATION_SUPPORT)
    set(ASSETS_LANGUAGES "${CMAKE_SOURCE_DIR}/assets/languages")
endif()

if(PSP)
    set(ASSETS_BACKGROUNDS "${CMAKE_CURRENT_SOURCE_DIR}/platform/psp/assets/backgrounds")
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
    if(TRANSLATION_SUPPORT)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${ASSETS_LANGUAGES} ${CMAKE_CURRENT_BINARY_DIR}/assets/languages)
    endif()
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_SOURCE_DIR}/assets/CREDITS.md" ${CMAKE_CURRENT_BINARY_DIR}/assets)
endif()

# Platform specific install
if (VITA)
    include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)
    vita_create_self(${CMAKE_PROJECT_NAME}.self ${CMAKE_PROJECT_NAME})

    vita_create_vpk(${CMAKE_PROJECT_NAME}.vpk "OPOP00001" ${CMAKE_PROJECT_NAME}.self
        VERSION "01.00"
        NAME OceanPop
        FILE platform/vita/sce_sys sce_sys
        FILE CREDITS.md CREDITS.md
        FILE "${ASSETS_BACKGROUNDS}" assets/backgrounds
        FILE "${ASSETS_FONTS}" assets/fonts
        FILE "${ASSETS_IMAGES}" assets/images
        FILE "${ASSETS_LEVELS}" assets/levels
        FILE "${ASSETS_MUSIC}" assets/music
        FILE "${ASSETS_SOUNDS}" assets/sounds
    )
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
