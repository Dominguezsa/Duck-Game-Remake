#include "game_client.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2pp/Chunk.hh>
#include <SDL2pp/Music.hh>

#include "../common/types/duck_state.h"

#include "lobby.h"

#define FPS 60
#define OST_VOLUME 2
#define SFX_VOLUME 5

#define GRAPHIC_QUEUE_SIZE 50


GameClient::GameClient(const int window_width, const int window_height,
                       const std::string& window_title, const int max_chunk_size_audio,
                       const std::string& server_ip, const std::string& port):
        quit(false),
        sdl(SDL_INIT_VIDEO),
        ttf(),
        mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS,
              max_chunk_size_audio),
        window(window_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width,
               window_height, SDL_WINDOW_SHOWN),
        renderer(window, -1, SDL_RENDERER_ACCELERATED),
        resourceManager(renderer),
        socket(server_ip.c_str(), port.c_str()),
        lobby(socket),
        messagesForServer(),
        graphic_queue(GRAPHIC_QUEUE_SIZE),
        animationHelper(resourceManager),
        screenRenderer(renderer, resourceManager, animationHelper),
        keyboardState(std::make_shared<const uint8_t*>(SDL_GetKeyboardState(nullptr))),
        commandCenter(messagesForServer, keyboardState, quit),
        playerAmount(0),
        audioEngine(snapshot.ducks, mixer, resourceManager) {}

void GameClient::run() {
    std::vector<Platform> platforms;
    ClientProtocol protocol(socket);
    ThreadReceiver threadReceiver(protocol, graphic_queue);
    ThreadSender threadSender(protocol, messagesForServer);
    threadReceiver.receivePlatforms(platforms);
    snapshot.platforms = platforms;
    threadReceiver.start();
    threadSender.start();
    std::cout << "CLIENT: Starting the UI \n";

    Snapshot ducksStates = graphic_queue.pop();  // This is the first snapshot

    playerAmount = ducksStates.ducks.size();

    resourceManager.loadResources(playerAmount);

    for (int i = 0; i < playerAmount; i++) {
        snapshot.ducks.push_back(DuckState());
    }

    animationHelper.loadDucks(snapshot.ducks);

    int64_t rate = 1000 / FPS;
    auto t1 = std::chrono::high_resolution_clock::now();
    int64_t t1_ms =
            std::chrono::duration_cast<std::chrono::milliseconds>(t1.time_since_epoch()).count();

    mixer.SetMusicVolume(OST_VOLUME);
    mixer.SetVolume(-1, SFX_VOLUME);
    auto musicTrack = resourceManager.getMusicTrack("back_music");
    mixer.PlayMusic(*musicTrack, -1);

    try {
        int iteration = 0;
        while (true) {
            updateDuckStates();
            mainLoop(iteration);
            if (round_finished()) {
                std::cout << "CLIENT: Round finished\n";
                screenRenderer.show_next_round();
                snapshot.bullets.clear();
                sleep(1);
            }

            if (quit) {
                break;
            }

            auto t2 = std::chrono::high_resolution_clock::now();
            int64_t t2_ms =
                    std::chrono::duration_cast<std::chrono::milliseconds>(t2.time_since_epoch())
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
        }
    } catch (...) {
        std::cout << "Error in the main loop\n";
        std::cout << "exiting the game by error\n";
    }
    threadReceiver.stop_thread();
    threadSender.stop_thread();
}

bool GameClient::round_finished() {
    int alive_ducks = std::count_if(snapshot.ducks.begin(), snapshot.ducks.end(),
                                    [](const DuckState& duck) { return duck.is_alive; });
    return alive_ducks <= 1;
}

void GameClient::run_lobby() { lobby.run(); }


void GameClient::updateDuckStates() {

    Snapshot snapshot_from_queue;
    try {

        while (graphic_queue.try_pop(snapshot_from_queue)) {}

        snapshot.updateSnapshot(snapshot_from_queue.ducks, snapshot_from_queue.bullets,
                                snapshot_from_queue.weapons);
    } catch (...) {
        std::cout << "Error updating duck states\n";
        std::cout << "exiting the game by error\n";
        throw;
    }
}

void GameClient::mainLoop(const int it) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN && event.key.repeat) {
            continue;
        }
        if (event.type == SDL_KEYDOWN) {
            animationHelper.set_run_anim_start(it);
        }
        commandCenter.processEvent(event);
    }

    screenRenderer.updateScreen(snapshot, it);
    audioEngine.playAudio();
}


GameClient::~GameClient() {}
