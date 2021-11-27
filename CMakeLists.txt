cmake_minimum_required(VERSION 3.10)

project(match-theory VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
     add_compile_options(-Wall -Wextra -pedantic)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
     if(CMAKE_BUILD_TYPE STREQUAL "Release")
          add_compile_options("")
     endif()
endif()

file(GLOB_RECURSE SRC src/*.cpp)
if(WIN32)
     add_executable(match-theory WIN32 ${SRC})
else()
     add_executable(match-theory ${SRC})
endif()

if (VITA)
     # VITASDK defines
     if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
          if(DEFINED ENV{VITASDK})
               set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
               else()
               message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
               endif()
     endif()

     include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)


     # Project start
     set(VITA_APPNAME Match-Theory)
     set(VITA_TITLEID "MATH00001")
     set(VITA_VERSION "01.00")

     set(VITA_INCLUDE_DIR "$ENV{VITASDK}/arm-vita-eabi/include/")
     set(SDL2_INCLUDE_DIR "$ENV{VITASDK}/arm-vita-eabi/include/SDL2")
     set(JSON_INCLUDE_DIR "$ENV{VITASDK}/arm-vita-eabi/include/jsoncpp")

     target_include_directories(match-theory PRIVATE
          ${VITA_INCLUDE_DIR}
          ${SDL2_INCLUDE_DIR}
          ${JSON_INCLUDE_DIR}
     )

     target_link_libraries(match-theory
          SDL2_image
          SDL2_ttf
          SDL2_mixer
          SDL2
          jsoncpp
          stdc++
          webp
          pthread
          png16
          freetype
          vorbisfile
          vorbis
          ogg
          jpeg
          z
          m
          mikmod
          mpg123
          FLAC
          SceAppUtil_stub
          SceAudio_stub
          SceCommonDialog_stub
          SceCtrl_stub
          SceDisplay_stub
          SceGxm_stub
          SceHid_stub
          SceSysmodule_stub
          SceTouch_stub
          SceMotion_stub
     )

     vita_create_self(match-theory.self match-theory)

     vita_create_vpk(match-theory.vpk ${VITA_TITLEID} match-theory.self
          VERSION ${VITA_VERSION}
          NAME ${VITA_APPNAME}
          FILE platform/vita/sce_sys sce_sys
          FILE platform/vita/assets/images assets/images
          FILE platform/vita/assets/backgrounds assets/backgrounds
          FILE assets/fonts assets/fonts
     )
ELSEIF(PLATFORM_PSP)
     set(SDL2_INCLUDE_DIR "${PSPDEV}/psp/include/SDL2")

     target_include_directories(match-theory PRIVATE
          ${SDL2_INCLUDE_DIR}
     )

     target_link_libraries(match-theory PRIVATE
          SDL2_image
          SDL2_mixer
          SDL2main
          SDL2
          SDL2_ttf
          png
          jpeg
          pspvram
          GL
          pspvram
          pspvfpu
          pspgu
          pspdebug
          pspdisplay
          pspge
          pspaudio
          psphprm
          psprtc
          jsoncpp
          vorbisfile
          vorbis
          ogg
          freetype
          z
     )

     create_pbp_file(
          TARGET match-theory
          ICON_PATH NULL
          BACKGROUND_PATH NULL
          PREVIEW_PATH NULL
          TITLE Match-Theory
          ENC
          BUILD_PRX
     )

     file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/assets)
     file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/fonts
          DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets)
     file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/platform/psp/assets/images
          DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets)
     file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/platform/psp/assets/backgrounds
          DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets)

ELSEIF(WIN32)
     find_package(SDL2 CONFIG REQUIRED)
     find_package(sdl2-image CONFIG REQUIRED)
     find_package(sdl2-ttf CONFIG REQUIRED)
     find_package(sdl2-mixer CONFIG REQUIRED)
     find_package(jsoncpp CONFIG REQUIRED)

     target_link_libraries(match-theory PRIVATE
          SDL2::SDL2
          SDL2::SDL2main
          SDL2::SDL2_image
          SDL2::SDL2_ttf
          SDL2::SDL2_mixer
          jsoncpp_lib
          jsoncpp_object
          JsonCpp::JsonCpp
     )
     target_include_directories(match-theory PRIVATE
          ${SDL2_INCLUDE_DIRS}
          ${SDL2_IMAGE_INCLUDE_DIRS}
          ${SDL2_TTF_INCLUDE_DIRS}
          ${SDL2_MIXER_INCLUDE_DIRS}
          ${JSONCPP_INCLUDE_DIRS}
     )
     if(NOT CMAKE_BUILD_TYPE MATCHES "Release")
         set(CMAKE_BUILD_TYPE "Debug")
     endif()
     
     file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets
          DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/)
ELSE()
     include(FindPkgConfig)

     pkg_search_module(SDL2 REQUIRED sdl2)
     pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image >= 2.0.0)
     pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)

     target_include_directories(match-theory PRIVATE ${SDL2_INCLUDE_DIRS})
     
     target_link_libraries(match-theory
          ${SDL2_TTF_LIBRARIES}
          ${SDL2_IMAGE_LIBRARIES}
          ${SDL2_LIBRARIES}
     )

     file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets
          DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
ENDIF()
