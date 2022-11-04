cmake_minimum_required(VERSION 3.12)

project(oceanpop)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
     add_compile_options(-Wall -Wextra -pedantic)
endif()

file(GLOB_RECURSE SRC src/*.cpp)
if(WIN32)
     add_executable(${CMAKE_PROJECT_NAME} WIN32 ${SRC} ${CMAKE_CURRENT_SOURCE_DIR}/platform/windows/appicon.rc)
else()
     add_executable(${CMAKE_PROJECT_NAME} ${SRC})
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
     set(VITA_APPNAME OceanPop)
     set(VITA_TITLEID "OPOP00001")
     set(VITA_VERSION "01.00")

     set(VITA_INCLUDE_DIR "$ENV{VITASDK}/arm-vita-eabi/include/")
     set(SDL2_INCLUDE_DIR "$ENV{VITASDK}/arm-vita-eabi/include/SDL2")
     set(JSON_INCLUDE_DIR "$ENV{VITASDK}/arm-vita-eabi/include/jsoncpp")

     target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
          ${VITA_INCLUDE_DIR}
          ${SDL2_INCLUDE_DIR}
          ${JSON_INCLUDE_DIR}
     )

     target_link_libraries(${CMAKE_PROJECT_NAME}
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
	  modplug
          mikmod
          mpg123
          FLAC
          SceAppUtil_stub
          SceAudio_stub
	  SceAudioIn_stub
          SceCommonDialog_stub
          SceCtrl_stub
          SceDisplay_stub
          SceGxm_stub
          SceHid_stub
          SceSysmodule_stub
          SceTouch_stub
          SceMotion_stub
     )

     vita_create_self(${CMAKE_PROJECT_NAME}.self ${CMAKE_PROJECT_NAME})

     vita_create_vpk(${CMAKE_PROJECT_NAME}.vpk ${VITA_TITLEID} ${CMAKE_PROJECT_NAME}.self
          VERSION ${VITA_VERSION}
          NAME ${VITA_APPNAME}
          FILE platform/vita/sce_sys sce_sys
          FILE platform/vita/assets/backgrounds assets/backgrounds
	  FILE README.md README.md
	  FILE LICENSE.txt LICENSE.txt
          FILE assets/images assets/images
          FILE assets/fonts assets/fonts
          FILE assets/sounds assets/sounds
          FILE assets/music assets/music
          FILE assets/levels assets/levels
     )
elseif(PLATFORM_PSP)
     set(SDL2_INCLUDE_DIR "${PSPDEV}/psp/include/SDL2")

     target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
          ${SDL2_INCLUDE_DIR}
     )

     target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
          SDL2_image
          SDL2_mixer
          SDL2
          SDL2_ttf
          SDL2main
          png
          jpeg
          pspvram
          GL
          pspvram
          pspvfpu
          pspgu
          pspctrl
          pspdebug
          pspdisplay
          pspge
          pspaudio
          psphprm
          jsoncpp
          vorbisfile
          vorbis
          ogg
	  xmp
          freetype
	  bz2
          z
     )

     create_pbp_file(
          TARGET ${CMAKE_PROJECT_NAME}
          ICON_PATH ${CMAKE_SOURCE_DIR}/platform/psp/icon.png
          BACKGROUND_PATH NULL
          PREVIEW_PATH NULL
          TITLE OceanPop
     )

     file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/assets)
     file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/fonts
          DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets)
     file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/sounds
          DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets)
     file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/levels
          DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets)
     file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/images
          DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets)
     file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/platform/psp/assets/music
          DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets)
     file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/platform/psp/assets/backgrounds
          DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets)

elseif(WIN32)
     option(BUILD_STATIC "Build a static binary (Windows only)" OFF)

    find_package(jsoncpp CONFIG REQUIRED)
    find_package(SDL2 CONFIG REQUIRED)
    find_package(sdl2-image CONFIG REQUIRED)
    find_package(SDL2_mixer CONFIG REQUIRED)
    find_package(SDL2_ttf CONFIG REQUIRED)

     target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
          SDL2::SDL2
          SDL2::SDL2main
          SDL2::SDL2_image
	  SDL2_ttf::SDL2_ttf
          SDL2_mixer::SDL2_mixer
          jsoncpp_object
          JsonCpp::JsonCpp
     )

     if(BUILD_STATIC)
          if(MSVC)
               set(CompilerFlags
                    CMAKE_CXX_FLAGS
                    CMAKE_CXX_FLAGS_DEBUG
                    CMAKE_CXX_FLAGS_RELEASE
                    CMAKE_CXX_FLAGS_MINSIZEREL
                    CMAKE_CXX_FLAGS_RELWITHDEBINFO
               )
               foreach(CompilerFlag ${CompilerFlags})
                    string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
                    set(${CompilerFlag} "${${CompilerFlag}}" CACHE STRING "msvc compiler flags" FORCE)
                    message("MSVC flags: ${CompilerFlag}:${${CompilerFlag}}")
               endforeach()
          endif()
          target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
               SDL2::SDL2-static
	       SDL2_mixer::SDL2_mixer-static
	       SDL2_ttf::SDL2_ttf-static
               jsoncpp_static
               Shlwapi
          )
     endif()

     target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
          ${SDL2_INCLUDE_DIRS}
          ${SDL2_IMAGE_INCLUDE_DIRS}
          ${SDL2_TTF_INCLUDE_DIRS}
          ${SDL2_MIXER_INCLUDE_DIRS}
          ${JSONCPP_INCLUDE_DIRS}
     )
else()
     include(FindPkgConfig)

     pkg_search_module(SDL2 REQUIRED sdl2)
     pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image >= 2.0.0)
     pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)
     pkg_search_module(SDL2_MIXER REQUIRED SDL2_mixer)
     pkg_search_module(JSONCPP REQUIRED jsoncpp)

     target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS} ${JSONCPP_INCLUDE_DIRS})
     
     target_link_libraries(${CMAKE_PROJECT_NAME}
          ${SDL2_TTF_LIBRARIES}
          ${SDL2_IMAGE_LIBRARIES}
          ${SDL2_MIXER_LIBRARIES}
          ${SDL2_LIBRARIES}
          ${JSONCPP_LIBRARIES}
     )

     file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets
          DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

     # Set install paths
     install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION bin)
     install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/assets DESTINATION share/${CMAKE_PROJECT_NAME})
     install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/platform/unix/${CMAKE_PROJECT_NAME}.desktop DESTINATION share/applications)
     install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/platform/unix/icons/128x128/${CMAKE_PROJECT_NAME}.png DESTINATION share/icons/hicolor/128x128/apps)
     install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/README.md DESTINATION share/doc/${CMAKE_PROJECT_NAME})
endif()
