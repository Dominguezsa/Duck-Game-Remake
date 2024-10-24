#include <exception>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "game_client.h"
// #include "common/foo.h"

// using SDL2pp::Renderer;
// using SDL2pp::SDL;
// using SDL2pp::Window;


// #define WINDOW_NAME "DuckGame"
// #define WINDOW_WIDTH 1200
// #define WINDOW_HEIGHT 700

// #define CHUNK_SIZE_AUDIO 409600


int main() try {

    const std::string WINDOW_NAME = "DuckGame";
    const int WINDOW_WIDTH = 1200;
    const int WINDOW_HEIGHT = 700;
    const int CHUNK_SIZE_AUDIO = 409600;

    GameClient gameClient(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, CHUNK_SIZE_AUDIO);

    gameClient.run();

    // // Initialize SDL library
    // SDL sdl(SDL_INIT_VIDEO);

    // // Create main window: 640x480 dimensions, resizable, "SDL2pp demo" title
    // Window window("SDL2pp demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
    //               SDL_WINDOW_RESIZABLE);

    // // Create accelerated video renderer with default driver
    // Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

    // // Clear screen
    // renderer.Clear();

    // // Show rendered frame
    // renderer.Present();

    // // 5 second delay
    // SDL_Delay(5000);

    // Here all resources are automatically released and library deinitialized
    return 0;
} catch (std::exception& e) {
    // If case of error, print it and exit with error
    std::cerr << e.what() << std::endl;
    return 1;
}
