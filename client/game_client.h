#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

#include <memory>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2pp/Chunk.hh>
#include <SDL2pp/Font.hh>
#include <SDL2pp/Mixer.hh>
#include <SDL2pp/Music.hh>
#include <SDL2pp/Renderer.hh>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/SDLTTF.hh>
#include <SDL2pp/Texture.hh>
#include <SDL2pp/Window.hh>

#include "../common/common_queue.h"
#include "../common/common_socket.h"
#include "../common/duck.h"
#include "../common/types/duck_state.h"

#include "AnimationHelper.h"
#include "ResourceManager.h"
#include "ScreenRenderer.h"
#include "lobby.h"
#include "protocol.h"
#include "thread_receiver.h"
#include "thread_sender.h"

class GameClient {
private:
    SDL2pp::SDL sdl;
    SDL2pp::SDLTTF ttf;
    SDL2pp::Mixer mixer;
    SDL2pp::Window window;
    SDL2pp::Renderer renderer;
    ResourceManager resourceManager;
    Socket socket;
    Lobby lobby;
    ClientProtocol protocol;
    Queue<uint8_t> messagesForServer;
    Queue<std::vector<DuckState>> graphic_queue;
    std::vector<Duck> ducks;  // cppcheck-suppress unusedStructMember
    AnimationHelper animationHelper;
    ScreenRenderer screenRenderer;
    // Algo específico de SDL, no creo que está en la librería SDL2pp
    std::unique_ptr<const uint8_t*> keyboardState;

    // DuckState stateDuck1;
    // DuckState stateDuck2;


public:
    GameClient(const int window_width, const int window_height, const std::string& window_title,
               const int max_chunk_size_audio, const std::string& server_ip,
               const std::string& port);
    ~GameClient();
    void run();
    void run_lobby();
    void updateDuckStates();
    void processEvent(const SDL_Event& event, bool& quit, int it);
    void mainLoop(const int it, bool& quit);
};

#endif
