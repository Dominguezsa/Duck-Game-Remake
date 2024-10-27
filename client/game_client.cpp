#include "game_client.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2pp/Chunk.hh>
#include <SDL2pp/Music.hh>

// #include "../common/common_constantRateLoop.cpp"

#include <chrono>
#include <thread>

#define FPS 60
struct Platform {
    float x;
    float y;
    // cppcheck-suppress unusedStructMember
    float width;
    // cppcheck-suppress unusedStructMember
    float height;
};
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
        resourceManager(renderer),
        socket("localhost", "8080"),
        protocol(socket),
        messagesForServer(),
        graphique_queue() {}

// Moved the constantRateLoop implementation here because making it a separate file was causing a
// lot of problems, but yeah its repeating the same code in both client and server
void GameClient::run() {
    ThreadReceiver threadReceiver(protocol, graphique_queue);
    ThreadSender threadSender(protocol, messagesForServer);
    threadReceiver.start();
    threadSender.start();


    // Load resources
    resourceManager.loadResources();

    int64_t rate = 1000 / FPS;
    int iteration = 0;
    renderer.Copy(*resourceManager.getTexture("background"), SDL2pp::NullOpt, SDL2pp::NullOpt);
    int cell_width = renderer.GetOutputWidth() / 12;
    int cell_height = renderer.GetOutputHeight() / 14;
    Platform left_platform = {0, (float)renderer.GetOutputHeight() / 2,
                              (float)renderer.GetOutputWidth() / 2, (float)cell_height};

    Platform right_platform = {(float)renderer.GetOutputWidth() / 2,
                               (float)renderer.GetOutputHeight() / 2 + cell_height * 3,
                               (float)renderer.GetOutputWidth() / 2, (float)cell_height};
    for (int i = 0; i < 6; ++i) {
        renderer.Copy(*resourceManager.getTexture("tablas"), SDL2pp::NullOpt,
                      SDL2pp::Rect(i * cell_width, left_platform.y, cell_width, cell_height));
    }

    // Right platform
    for (int i = 0; i < 6; ++i) {
        renderer.Copy(*resourceManager.getTexture("tablas"), SDL2pp::NullOpt,
                      SDL2pp::Rect(i * cell_width + right_platform.x, right_platform.y, cell_width,
                                   cell_height));
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    int64_t t1_ms =
            std::chrono::duration_cast<std::chrono::milliseconds>(t1.time_since_epoch()).count();

    mixer.SetMusicVolume(0);
    mixer.SetVolume(-1, 20);
    auto musicTrack = resourceManager.getMusicTrack("back_music");
    mixer.PlayMusic(*musicTrack, -1);

    bool quit = false;

    // std::cout << "Starting, this should give me a loop of 1 iteration per " << rate << "ms\n";
    std::cout << "Drawing the background and both platforms\n";
    renderer.Present();

    while (true) {

        mainLoop(iteration, quit);

        if (quit) {
            std::cout << "Exiting the game\n";
            break;
        }


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
        // SDL_Delay(rest);
        std::this_thread::sleep_for(std::chrono::milliseconds(rest));
        t1_ms += rate;
        iteration += 1;
        // std::cout << "Iteration: " << iteration << "\n";
    }
    std::cout << "CLIENT: Stopping the receiver thread\n";
    threadReceiver.stop_thread();
    std::cout << "CLIENT: Closing the graphic queue\n";
    graphique_queue.close();
    std::cout << "CLIENT: Stopping the sender thread\n";
    threadSender.stop_thread();
    std::cout << "CLIENT: Closing the messages for server queue\n";
    messagesForServer.close();
}

void GameClient::mainLoop(const int it, bool& quit) {
    // Now it should do everything that the game needs to do in one iteration, like play if it needs
    // to play music, render sprites, etc.

    // Now processing all of the events on this frame
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            quit = true;
        } else if (event.type == SDL_KEYDOWN) {
            std::cout << "Caught a keydown event on iteration: " << it << "\n";
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                case SDLK_1:
                    mixer.PlayChannel(-1, *resourceManager.getSFX("boom1"), 1);
                    break;
                case SDLK_2:
                    mixer.PlayChannel(-1, *resourceManager.getSFX("boom2"), 0);
                    break;
                case SDLK_3:
                    mixer.PlayChannel(-1, *resourceManager.getSFX("boom3"), 0);
                    break;
                case SDLK_d:
                    messagesForServer.push(0x01);
                    break;
                case SDLK_a:
                    messagesForServer.push(0x03);
                    break;
                case SDLK_SPACE:
                    messagesForServer.push(0x05);
                    break;
                case SDLK_f:
                    messagesForServer.push(0x07);
                    break;
                default:
                    break;
            }
        } else if (event.type == SDL_KEYUP) {
            std::cout << "Caught a keyup event on iteration: " << it << "\n";
            switch (event.key.keysym.sym) {
                case SDLK_d:
                    messagesForServer.push(0x02);
                    break;
                case SDLK_a:
                    messagesForServer.push(0x04);
                    break;
                case SDLK_SPACE:
                    messagesForServer.push(0x06);
                    break;
                case SDLK_f:
                    messagesForServer.push(0x08);
                    break;
                default:
                    break;
            }
        }
    }
}


GameClient::~GameClient() {}
