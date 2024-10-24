#include "game_client.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2pp/Chunk.hh>
#include <SDL2pp/Music.hh>

#include "../common/common_constantRateLoop.cpp"

GameClient::GameClient(const int window_width, const int window_height,
                       const std::string& window_title, const int max_chunk_size_audio):
        sdl(SDL_INIT_VIDEO),
        ttf(),
        mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS,
              max_chunk_size_audio),
        window(window_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width,
               window_height, SDL_WINDOW_RESIZABLE),
        renderer(window, -1, SDL_RENDERER_ACCELERATED) {}

void GameClient::run() {}


GameClient::~GameClient() {}
