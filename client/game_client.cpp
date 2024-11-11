#include "game_client.h"

#include <chrono>
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

#include "../common/common_weapon.h"
#include "../common/types/action_type.h"
#include "../common/types/duck_state.h"

#include "lobby.h"

#define FPS 60
#define OST_VOLUME 0
#define SFX_VOLUME 20

#define GRAPHIC_QUEUE_SIZE 50


// Loads the game client with all of the necessary main resources (in the future maybe it will be
// even more)
GameClient::GameClient(const int window_width, const int window_height,
                       const std::string& window_title, const int max_chunk_size_audio,
                       const std::string& server_ip, const std::string& port):
        sdl(SDL_INIT_VIDEO),
        ttf(),
        mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS,
              max_chunk_size_audio),
        window(window_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width,
               window_height, SDL_WINDOW_RESIZABLE),
        renderer(window, -1, SDL_RENDERER_ACCELERATED),
        resourceManager(renderer),
        socket(server_ip.c_str(), port.c_str()),
        lobby(socket),
        messagesForServer(),
        graphic_queue(GRAPHIC_QUEUE_SIZE),
        ducks({Duck(), Duck()}),
        animationHelper(ducks, resourceManager),
        screenRenderer(renderer, resourceManager, animationHelper),
        keyboardState(std::make_unique<const uint8_t*>(SDL_GetKeyboardState(nullptr))) {}

// Moved the constantRateLoop implementation here because making it a separate file was causing a
// lot of problems, but yeah its repeating the same code in both client and server
void GameClient::run() {
    ClientProtocol protocol(socket);
    ThreadReceiver threadReceiver(protocol, graphic_queue);
    ThreadSender threadSender(protocol, messagesForServer);
    threadReceiver.start();
    threadSender.start();


    // Load resources
    resourceManager.loadResources();

    int64_t rate = 1000 / FPS;
    int iteration = 0;
    auto t1 = std::chrono::high_resolution_clock::now();
    int64_t t1_ms =
            std::chrono::duration_cast<std::chrono::milliseconds>(t1.time_since_epoch()).count();

    mixer.SetMusicVolume(OST_VOLUME);
    mixer.SetVolume(-1, SFX_VOLUME);
    auto musicTrack = resourceManager.getMusicTrack("back_music");
    mixer.PlayMusic(*musicTrack, -1);

    bool quit = false;

    while (true) {
        updateDuckStates();
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
    }
    std::cout << "CLIENT: Stopping the receiver thread\n";
    threadReceiver.stop_thread();
    std::cout << "CLIENT: Closing the graphic queue\n";
    graphic_queue.close();
    std::cout << "CLIENT: Stopping the sender thread\n";
    threadSender.stop_thread();
    std::cout << "CLIENT: Closing the messages for server queue\n";
    messagesForServer.close();
}

void GameClient::run_lobby() { lobby.run(); }


void GameClient::updateDuckStates() {

    std::vector<DuckState> duck_states;


    while (graphic_queue.try_pop(duck_states)) {}

    for (int i = 0; i < (int)duck_states.size(); i++) {
        // std::cout << "updated ducks\n";
        ducks[i].update_state(duck_states[i]);
    }
}

void GameClient::processEvent(const SDL_Event& event, bool& quit, int it) {
    // Esto debería ser algo tipo double dispatch porque es un asco así
    if (event.type == SDL_QUIT) {
        quit = true;
    } else if (event.type == SDL_KEYDOWN) {
        animationHelper.set_run_anim_start(it);

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
                messagesForServer.push(MOVE_RIGHT_KEY_DOWN);
                break;
            case SDLK_a:
                messagesForServer.push(MOVE_LEFT_KEY_DOWN);
                break;
            case SDLK_w:
                messagesForServer.push(LOOKING_UP_KEY_DOWN);
                break;
            case SDLK_s:
                messagesForServer.push(LOOKING_DOWN_KEY_DOWN);
                break;
            case SDLK_SPACE:
                messagesForServer.push(JUMP_KEY_DOWN);
                break;
            case SDLK_f:
                messagesForServer.push(SHOOT_KEY_DOWN);
                break;
            default:
                break;
        }
    } else if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
            case SDLK_d:
                if ((*keyboardState)[SDL_SCANCODE_A]) {
                    messagesForServer.push(LOOKING_LEFT_KEY_DOWN);
                    break;
                }
                messagesForServer.push(MOVE_RIGHT_KEY_UP);
                break;
            case SDLK_a:
                if ((*keyboardState)[SDL_SCANCODE_D]) {
                    messagesForServer.push(LOOKING_RIGHT_KEY_DOWN);
                    break;
                }
                messagesForServer.push(MOVE_LEFT_KEY_UP);
                break;
            case SDLK_w:
                messagesForServer.push(LOOKING_UP_KEY_UP);
                break;
            case SDLK_s:
                messagesForServer.push(LOOKING_DOWN_KEY_UP);
                break;
            case SDLK_SPACE:
                messagesForServer.push(JUMP_KEY_UP);
                break;
            case SDLK_f:
                messagesForServer.push(SHOOT_KEY_UP);
                break;
            default:
                break;
        }
    }
}

void GameClient::mainLoop(const int it, bool& quit) {
    // Now it should do everything that the game needs to do in one iteration, like play if it needs
    // to play music, render sprites, etc.

    // Now processing all of the events on this frame
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN && event.key.repeat) {
            continue;
        }
        processEvent(event, quit, it);
    }

    screenRenderer.updateScreen(ducks, it);
}


GameClient::~GameClient() {}
