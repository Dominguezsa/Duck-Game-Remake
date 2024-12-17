cmake_minimum_required(VERSION 3.13)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

set(CMAKE_CXX_EXTENSIONS ON)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
endif()

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example
# when compiling with PCH enabled
set(CMAKE_CXX_EXTENSIONS ON)

if(POLICY CMP0100)
    cmake_policy(SET CMP0100 NEW)
endif()

# Set compiler flags conditionally
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    # Release mode flags (optimizations enabled)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -pedantic -pedantic-errors -O3 -fno-inline -D _POSIX_C_SOURCE=200809L")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic -pedantic-errors -O3 -fno-inline -D _POSIX_C_SOURCE=200809L")
    message("-- Compiling in release mode (optimizations enabled)")
    add_compile_definitions(RESOURCE_PATH="/var/duck_game")
else()
    # Debug mode flags (no optimization, debugging info)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -pedantic -pedantic-errors -O0 -ggdb -DDEBUG -fno-inline -D _POSIX_C_SOURCE=200809L")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic -pedantic-errors -O0 -g3 -ggdb -DDEBUG -fno-inline -D _POSIX_C_SOURCE=200809L")
    message("-- Compiling in debug mode (no optimizations, debugging info)")
    add_compile_definitions(RESOURCE_PATH="${CMAKE_CURRENT_LIST_DIR}/resources")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=return-type")

project(taller_tp
    VERSION 1.0
    HOMEPAGE_URL "https://github.com/URL-HERE"
    LANGUAGES CXX
)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

option(TALLER_TESTS "Enable / disable tests." ON)
option(DUCK_GAME "Enable / disable client program." ON)
option(DUCK_GAME_SERVER "Enable / disable server program." ON)
option(MAP_EDITOR "Enable / disable editor program." ON)
option(TALLER_MAKE_WARNINGS_AS_ERRORS "Enable / disable warnings as errors." ON)


# Find Qt5 package
find_package(Qt5 COMPONENTS Widgets REQUIRED)


# Automatic generation of UI files from Qt turned on
set(CMAKE_AUTOUIC ON)
# Automatic generation of .moc (Meta-Object Compiler) files from Qt turned on
set(CMAKE_AUTOMOC ON)
# Automatic generation of resources (.qrc) from Qt turned on
set(CMAKE_AUTORCC ON)

# Find Qt neccesary packages
find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets Gui Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia)
find_package(yaml-cpp REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Set the output directory for the build executables
message(CMAKE_CXX_COMPILER_ID= "${CMAKE_CXX_COMPILER_ID}")


add_library(common_libs STATIC)


add_subdirectory(common/)


# set compiler warnings to the targets
include(cmake/CompilerWarnings.cmake)
set_project_warnings(common_libs ${TALLER_MAKE_WARNINGS_AS_ERRORS} FALSE)

# Say where the headers will be found required by library target
# Make this PUBLIC so any dependant linking lib will inherit this
# location (so our dependants will not have to explicitly request this)
target_include_directories(common_libs PUBLIC .)


if(DUCK_GAME)   
    include(FetchContent)
    FetchContent_Declare(
        libSDL2pp
        URL https://github.com/libSDL2pp/libSDL2pp/archive/cc198c9a5657048bee67ece82de620b2d5661084.zip
    )

    # set up SDL2pp library
    set(SDL2PP_WITH_IMAGE YES)
    set(SDL2PP_WITH_TTF YES)
    set(SDL2PP_WITH_MIXER YES)


    FetchContent_MakeAvailable(libSDL2pp)
endif()

if (DUCK_GAME_SERVER OR DUCK_GAME)
    include(FetchContent)

    FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    yaml-cpp-lib
    )
    FetchContent_GetProperties(yaml-cpp)

    if(NOT yaml-cpp_POPULATED)
        message(STATUS "Fetching yaml-cpp...")
        FetchContent_Populate(yaml-cpp)
        add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})
    endif()
endif()


# Program section
# ---------------



# Update Qt components
find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets Gui Multimedia Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia Network)

if(DUCK_GAME)
    add_executable(duck_game)
    add_dependencies(duck_game common_libs SDL2pp::SDL2pp)
    
    # Add client source files
    add_subdirectory(client)

    # Include MainWindow and other editor files
    target_sources(duck_game
        PRIVATE
        editor/EditorWindow.cpp
        editor/EditorWindow.h
        editor/EditorWindow.ui
        editor/Tile.cpp
        editor/Tile.h
        editor/ImageWidget.cpp
        editor/ImageWidget.h 
    )

    set_project_warnings(duck_game ${TALLER_MAKE_WARNINGS_AS_ERRORS} FALSE)
    target_include_directories(duck_game PRIVATE ${libSDL2pp_SOURCE_DIR})

    target_link_libraries(duck_game 
        PRIVATE
        common_libs
        SDL2pp::SDL2pp
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Multimedia
        Qt${QT_VERSION_MAJOR}::Network
        yaml-cpp::yaml-cpp
        Threads::Threads
    )
endif()

if(DUCK_GAME_SERVER)
    add_executable(duck_game_server)
    add_dependencies(duck_game_server common_libs)
    add_subdirectory(server)
    set_project_warnings(duck_game_server ${TALLER_MAKE_WARNINGS_AS_ERRORS} FALSE)
    target_link_libraries(duck_game_server common_libs yaml-cpp::yaml-cpp)
endif()

if(MAP_EDITOR)
    add_executable(map_editor)

    target_link_libraries(map_editor 
        PRIVATE
        common_libs
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Multimedia
        Qt${QT_VERSION_MAJOR}::Network
        yaml-cpp::yaml-cpp
        Threads::Threads
    )

    add_subdirectory(editor)
    set_project_warnings(map_editor ${TALLER_MAKE_WARNINGS_AS_ERRORS} FALSE)
endif()



# Testing section
# ---------------

if(TALLER_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    include(GoogleTest)

    add_executable(taller_tests)
    add_dependencies(taller_tests common_libs)
    target_include_directories(taller_tests PUBLIC .)
    add_subdirectory(tests/)
    set_project_warnings(taller_tests ${TALLER_MAKE_WARNINGS_AS_ERRORS} TRUE)

    target_include_directories(taller_tests
        PUBLIC
        ${gtest_SOURCE_DIR}/include
        ${gmock_SOURCE_DIR}/include
    )

    target_link_libraries(taller_tests
        common_libs
        GTest::gtest_main
    )
endif()