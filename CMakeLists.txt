cmake_minimum_required(VERSION 3.10)

project(match-theory VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra -pedantic)

file(GLOB_RECURSE SRC src/*.cpp)
add_executable(match-theory ${SRC})

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

     target_include_directories(match-theory PRIVATE
          ${VITA_INCLUDE_DIR}
          ${SDL2_INCLUDE_DIR}
     )

     target_link_libraries(match-theory
          SDL2_image
          SDL2
          webp
          pthread
          png16
          jpeg
          z
          m
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
		    FILE assets/images images
	)
ELSEIF(PLATFORM_PSP)
     set(SDL2_INCLUDE_DIR "${PSPDEV}/psp/include/SDL2")

     target_include_directories(match-theory PRIVATE
          ${SDL2_INCLUDE_DIR}
     )

     target_link_libraries(match-theory PRIVATE SDL2_image SDL2 png jpeg z GLU GL pspvram pspvfpu psphprm pspdisplay pspgu pspge pspdebug pspaudio)

     create_pbp_file(
          TARGET match-theory
          ICON_PATH NULL
          BACKGROUND_PATH NULL
          PREVIEW_PATH NULL
          TITLE Match-Theory
          BUILD_PRX
     )

     file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/images
               DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
ELSE()
     include(FindPkgConfig)

     find_package(SDL2 REQUIRED)
     target_include_directories(match-theory PRIVATE ${SDL2_INCLUDE_DIRS})
     target_link_libraries(match-theory ${SDL2_LIBRARIES})

     pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image >= 2.0.0)
     target_include_directories(match-theory PRIVATE ${${SDL2_IMAGE_INCLUDE_DIRS}})
     target_link_libraries(match-theory ${SDL2_IMAGE_LIBRARIES})

     file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/images
          DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
ENDIF()