#include <exception>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "game_client.h"

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cout << "Usage: ./client <hostname> <servname>" << std::endl;
            return EXIT_FAILURE;
        }
        const std::string WINDOW_NAME = "DuckGame";
        const int WINDOW_WIDTH = 1200;
        const int WINDOW_HEIGHT = 700;
        const int CHUNK_SIZE_AUDIO = 409600;
        std::string hostname = std::string(argv[1]);
        std::string servname = std::string(argv[2]);

        GameClient gameClient(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, CHUNK_SIZE_AUDIO, hostname,
                              servname);
        gameClient.run();
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
