#include "game_client.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2pp/Chunk.hh>
#include <SDL2pp/Music.hh>

// #include "../common/common_constantRateLoop.cpp"

#include <chrono>
#include <thread>

#define FPS 60

// Loads the game client with all of the necessary main resources (in the future maybe it will be
// even more)
GameClient::GameClient(const int window_width, const int window_height,
                       const std::string& window_title, const int max_chunk_size_audio):
        sdl(SDL_INIT_VIDEO),
        ttf(),
        mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS,
              max_chunk_size_audio),
        window(window_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width,
               window_height, SDL_WINDOW_RESIZABLE),
        renderer(window, -1, SDL_RENDERER_ACCELERATED),
        resourceManager(renderer) {}

// Moved the constantRateLoop implementation here because making it a separate file was causing a
// lot of problems, but yeah its repeating the same code in both client and server
void GameClient::run() {

    // Load resources
    resourceManager.loadResources();

    int64_t rate = 1000 / FPS;
    int iteration = 0;

    auto t1 = std::chrono::high_resolution_clock::now();
    int64_t t1_ms =
            std::chrono::duration_cast<std::chrono::milliseconds>(t1.time_since_epoch()).count();

    mixer.SetMusicVolume(30);
    mixer.SetVolume(-1, 20);
    auto musicTrack = resourceManager.getMusicTrack("back_music");
    mixer.PlayMusic(*musicTrack, -1);

    // std::cout << "Starting, this should give me a loop of 1 iteration per " << rate << "ms\n";
    while (true) {

        mainLoop(iteration);

        auto t2 = std::chrono::high_resolution_clock::now();
        int64_t t2_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2.time_since_epoch())
                                .count();
        int64_t rest = rate - (t2_ms - t1_ms);
        if (rest < 0) {
            auto behind = -rest;
            rest = rate - behind % rate;
            auto lost = behind + rest;
            t1_ms += lost;
            iteration += lost / rate;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(rest));
        t1_ms += rate;
        iteration += 1;
    }
}

void GameClient::mainLoop(const int it) {
    // Now it should do everything that the game needs to do in one iteration, like play if it needs
    // to play music, render sprites, etc.
    std::cout << it << std::endl;
}


GameClient::~GameClient() {}
