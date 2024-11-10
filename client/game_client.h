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
#include "CommandCenter.h"
#include "ResourceManager.h"
#include "ScreenRenderer.h"
#include "protocol.h"
#include "thread_receiver.h"
#include "thread_sender.h"

class GameClient {
private:
    // cppcheck-suppress unusedStructMember
    bool quit;
    SDL2pp::SDL sdl;
    SDL2pp::SDLTTF ttf;
    SDL2pp::Mixer mixer;
    SDL2pp::Window window;
    SDL2pp::Renderer renderer;
    ResourceManager resourceManager;
    Socket socket;
    ClientProtocol protocol;
    Queue<uint8_t> messagesForServer;
    Queue<std::vector<DuckState>> graphic_queue;
    // cppcheck-suppress unusedStructMember
    std::vector<Duck> ducks;
    AnimationHelper animationHelper;
    ScreenRenderer screenRenderer;
    // Algo específico de SDL, no creo que está en la librería SDL2pp
    std::shared_ptr<const uint8_t*> keyboardState;
    CommandCenter commandCenter;


public:
    GameClient(const int window_width, const int window_height, const std::string& window_title,
               const int max_chunk_size_audio, const std::string& server_ip,
               const std::string& port);
    ~GameClient();
    void run();
    void updateDuckStates();
    void mainLoop(const int it);
};

#endif
