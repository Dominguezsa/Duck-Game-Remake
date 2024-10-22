#include "game_client.h"

#include <SDL2/SDL_mixer.h>

// Placeholder variables for the game initialization

#define WINDOW_NAME "DuckGame"
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 700

#define CHUNK_SIZE_AUDIO 409600


GameClient::GameClient():
        sdl(SDL_INIT_VIDEO),
        ttf(),
        mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, CHUNK_SIZE_AUDIO),
        window(WINDOW_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
               WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE),
        renderer(window, -1, SDL_RENDERER_ACCELERATED) {}

GameClient::~GameClient() {}
