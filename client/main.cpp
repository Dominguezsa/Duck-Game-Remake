#include <exception>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include "LobbyWindow.h"

#include "game_client.h"

#define WINDOW_NAME "DuckGame"
#define WINDOW_WIDTH 1250
#define WINDOW_HEIGHT 750
#define CHUNK_SIZE_AUDIO 2048


int main(int argc, char* argv[]) {
    try {
        std::string hostname, servname;
        GameClient gameClient(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, CHUNK_SIZE_AUDIO, hostname,
                              servname, argc, argv);
        gameClient.run_lobby();
        gameClient.run();

        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
