cmake_minimum_required(VERSION 3.13)

project(libSDL2pp LANGUAGES CXX VERSION 0.18.1)

# Options for enabling/disabling features
option(SDL2PP_WITH_IMAGE "Enable SDL2_image support" ON)
option(SDL2PP_WITH_MIXER "Enable SDL2_mixer support" ON)
option(SDL2PP_WITH_TTF "Enable SDL2_ttf support" ON)

find_package(SDL2 REQUIRED)

# Find the required SDL2 package
find_package(SDL2 REQUIRED)

set(SDL2_ALL_LIBRARIES SDL2::SDL2)
set(SDL2_ALL_PKGCONFIG_MODULES sdl2)

# Add sources for SDL2pp
file(GLOB LIBSDL2PP_SOURCES *.cpp *.h)

# Always link with the base SDL2 library
target_link_libraries(SDL2pp PUBLIC SDL2::SDL2)

# Conditional linking based on the chosen options
if(SDL2PP_WITH_IMAGE)
    find_package(SDL2_image REQUIRED)
	set(SDL2_ALL_LIBRARIES ${SDL2_ALL_LIBRARIES} SDL2_image::SDL2_image)
	set(SDL2_ALL_PKGCONFIG_MODULES "${SDL2_ALL_PKGCONFIG_MODULES} SDL2_image")
endif()

if(SDL2PP_WITH_MIXER)
    FIND_PACKAGE(SDL2_mixer REQUIRED)
    SET(SDL2_ALL_INCLUDE_DIRS ${SDL2_ALL_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIR})
    SET(SDL2_ALL_LIBRARIES ${SDL2_ALL_LIBRARIES} ${SDL2_MIXER_LIBRARY})
    SET(SDL2_ALL_PKGCONFIG_MODULES "${SDL2_ALL_PKGCONFIG_MODULES} SDL2_mixer")
endif()

if(SDL2PP_WITH_TTF)
    FIND_PACKAGE(SDL2_ttf REQUIRED)
    SET(SDL2_ALL_INCLUDE_DIRS ${SDL2_ALL_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIR})
    SET(SDL2_ALL_LIBRARIES ${SDL2_ALL_LIBRARIES} ${SDL2_TTF_LIBRARY})
    SET(SDL2_ALL_PKGCONFIG_MODULES "${SDL2_ALL_PKGCONFIG_MODULES} SDL2_ttf")
endif()

# Export the SDL2pp target for external usage
add_library(SDL2pp::SDL2pp ALIAS SDL2pp)

# Add the include directories so that consumers of SDL2pp can find headers
target_include_directories(SDL2pp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

LIST(REMOVE_DUPLICATES SDL2_ALL_INCLUDE_DIRS)
LIST(REMOVE_DUPLICATES SDL2_ALL_LIBRARIES)
