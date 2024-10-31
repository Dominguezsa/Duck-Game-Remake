#include "game_client.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2pp/Chunk.hh>
#include <SDL2pp/Music.hh>

#include "../common/types/duck_state.h"

// #include "../common/common_constantRateLoop.cpp"
#include <chrono>
#include <thread>

#include "../common/common_weapon.h"

#define FPS 60
#define OST_VOLUME 0
#define SFX_VOLUME 20

#define GRAPHIC_QUEUE_SIZE 50

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
        protocol(socket),
        messagesForServer(),
        graphic_queue(GRAPHIC_QUEUE_SIZE),
        duck1(),
        duck2()
// stateDuck1(0x00, Position(20, 40), 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, WeaponType(10)),
// stateDuck2(0x00, Position(60, 40), 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, WeaponType(10))
{}

// Moved the constantRateLoop implementation here because making it a separate file was causing a
// lot of problems, but yeah its repeating the same code in both client and server
void GameClient::run() {
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

    // std::cout << "Starting, this should give me a loop of 1 iteration per " << rate << "ms\n";
    // std::cout << "Drawing the background and both platforms\n";
    // renderer.Present();

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
        // std::cout << "Iteration: " << iteration << "\n";
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

void GameClient::updateDuckStates() {

    std::vector<DuckState> duck_states;


    while (graphic_queue.try_pop(duck_states)) {}

    // ugly but placeholder
    if (duck_states.size() == 1) {
        duck1.update_state(duck_states[0]);
    } else if (duck_states.size() == 2) {
        duck1.update_state(duck_states[0]);
        duck2.update_state(duck_states[1]);
    }
    // std::cout << "Duck state updated\n";
}

void GameClient::processEvent(const SDL_Event& event, bool& quit) {
    if (event.type == SDL_QUIT) {
        quit = true;
    } else if (event.type == SDL_KEYDOWN) {
        // std::cout << "Caught a keydown event on iteration: " << it << "\n";
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
                // if (startRunningItDuck1 != -1) {
                //     // isRunningDuck1 = true;
                // } else {
                //     // isRunningDuck1 = true;
                //     startRunningItDuck1 = it;
                // }
                // duckFacing = false;
                break;
            case SDLK_a:
                messagesForServer.push(0x03);
                // if (startRunningItDuck1 != -1) {
                //     // isRunningDuck1 = true;
                // } else {
                //     // isRunningDuck1 = true;
                //     // startRunningItDuck1 = it;
                // }
                // duckFacing = true;
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
        // std::cout << "Caught a keyup event on iteration: " << it << "\n";
        switch (event.key.keysym.sym) {
            case SDLK_d:
                messagesForServer.push(0x02);
                // startRunningItDuck1 = -1;
                // isRunningDuck1 = false;
                break;
            case SDLK_a:
                messagesForServer.push(0x04);
                // startRunningItDuck1 = -1;
                // isRunningDuck1 = false;
                // duckFacing = true;
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

void GameClient::mainLoop(const int it, bool& quit) {
    // Now it should do everything that the game needs to do in one iteration, like play if it needs
    // to play music, render sprites, etc.

    // Now processing all of the events on this frame


    // Esto debería ser algo tipo double dispatch de cabeza porque es un asco
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        processEvent(event, quit);
    }

    // Todo esto debería mínimo en una función aparte, muy probablemente en una clase aparte que se
    // encargue del renderizado

    renderer.Clear();

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

    // Now rendering the ducks
    // int animationPhase = ((it - startRunningItDuck1) / 6) % 6;
    // For now so it doesn't complain

    // Esto es la definición de código repetido, es un asco pero por ahora todo pelota

    // std::cout << "Animation phase: " << animationPhase << std::endl;
    SDL2pp::Rect frameDuck1;
    SDL2pp::Rect frameDuck2;

    DuckState duck1_state;
    DuckState duck2_state;

    duck1.get_state(duck1_state);
    duck2.get_state(duck2_state);

    // To check what iteration the duck started running to sync the animation from the start
    if (duck1_state.is_running && startRunningItDuck1 == -1) {
        // std::cout << "Setting startRunningItDuck1 to " << it << std::endl;
        startRunningItDuck1 = it;
    } else if (!duck1_state.is_running && startRunningItDuck1 != -1) {
        // std::cout << "Setting startRunningItDuck1 to -1 on it: " << it << std::endl;
        startRunningItDuck1 = -1;
    }

    if (duck2_state.is_running && startRunningItDuck2 == -1) {
        // std::cout << "Setting startRunningItDuck2 to " << it << std::endl;
        startRunningItDuck2 = it;
    } else if (!duck2_state.is_running && startRunningItDuck2 != -1) {
        startRunningItDuck2 = -1;
    }


    if (duck1_state.is_running) {
        int animationPhaseDuck1 = ((it - startRunningItDuck1) / 6) % 6;
        frameDuck1 = resourceManager.getAnimationFrame("duck_running", animationPhaseDuck1);
    } else {
        frameDuck1 = resourceManager.getAnimationFrame("duck_running", 0);
    }
    if (duck2_state.is_running) {
        int animationPhaseDuck2 = ((it - startRunningItDuck2) / 6) % 6;
        frameDuck2 = resourceManager.getAnimationFrame("duck_running", animationPhaseDuck2);
    } else {
        frameDuck2 = resourceManager.getAnimationFrame("duck_running", 0);
    }

    // Now lets render the ducks, taking into account the direction they are facing
    if (duck1_state.looking == 0) {
        renderer.Copy(*resourceManager.getTexture("white_duck"), frameDuck1,
                      SDL2pp::Rect(duck1_state.position.x, duck1_state.position.y, 62, 62), 0.0,
                      SDL2pp::NullOpt, SDL_FLIP_HORIZONTAL);
    } else {
        renderer.Copy(*resourceManager.getTexture("white_duck"), frameDuck1,
                      SDL2pp::Rect(duck1_state.position.x, duck1_state.position.y, 62, 62));
    }

    if (duck2_state.looking == 0) {
        renderer.Copy(*resourceManager.getTexture("orange_duck"), frameDuck2,
                      SDL2pp::Rect(duck2_state.position.x, duck2_state.position.y, 62, 62), 0.0,
                      SDL2pp::NullOpt, SDL_FLIP_HORIZONTAL);
    } else {
        renderer.Copy(*resourceManager.getTexture("orange_duck"), frameDuck2,
                      SDL2pp::Rect(duck2_state.position.x, duck2_state.position.y, 62, 62));
    }

    // Debug text

    std::string duck_position_text =
            "Position of duck_1: " + std::to_string((int)duck1_state.position.x) + ", " +
            std::to_string((int)duck1_state.position.y) + " Position of duck_2 " +
            std::to_string((int)duck2_state.position.x) + ", " +
            std::to_string((int)duck2_state.position.y) +
            " Duck 1 is running: " + std::to_string(duck1_state.is_running) +
            " Duck 2 is running: " + std::to_string(duck2_state.is_running) +
            " Duck 1 is facing: " + std::to_string(duck1_state.looking) +
            " Duck 2 is facing: " + std::to_string(duck2_state.looking);

    SDL2pp::Texture text_sprite(renderer,
                                resourceManager.getFont("vera")->RenderText_Blended(
                                        duck_position_text, SDL_Color{255, 255, 255, 255}));

    renderer.Copy(text_sprite, SDL2pp::NullOpt,
                  SDL2pp::Rect(0, 0, text_sprite.GetWidth(), text_sprite.GetHeight()));


    renderer.Present();
}


GameClient::~GameClient() {}
