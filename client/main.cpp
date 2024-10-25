#include <exception>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "game_client.h"

int main() try {

    const std::string WINDOW_NAME = "DuckGame";
    const int WINDOW_WIDTH = 1200;
    const int WINDOW_HEIGHT = 700;
    const int CHUNK_SIZE_AUDIO = 409600;

    GameClient gameClient(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, CHUNK_SIZE_AUDIO);
    gameClient.run();
    return EXIT_SUCCESS;

} catch (std::exception& e) {
    // If case of error, print it and exit with error
    std::cerr << e.what() << std::endl;
    return 1;
}
