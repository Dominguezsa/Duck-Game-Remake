#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

#include <QApplication>
#include <QProcess>
#include <QString>
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
#include "../common/snapshot.h"
#include "../common/types/duck_state.h"
#include "types/lobby_actions.h"

#include "AnimationHelper.h"
#include "AudioEngine.h"
#include "CommandCenter.h"
#include "LobbyWindow.h"
#include "ResourceManager.h"
#include "ScreenRenderer.h"
#include "WrapperLobbyApp.h"
#include "protocol.h"
#include "thread_receiver.h"
#include "thread_sender.h"

class GameClient {
private:
    bool quit;
    SDL2pp::SDL sdl;
    SDL2pp::SDLTTF ttf;
    SDL2pp::Mixer mixer;
    SDL2pp::Window window;
    SDL2pp::Renderer renderer;
    ResourceManager resourceManager;
    std::shared_ptr<Socket> socket;
    Queue<uint8_t> messagesForServer;
    Queue<Snapshot> graphic_queue;
    Snapshot snapshot;
    AnimationHelper animationHelper;
    ScreenRenderer screenRenderer;
    std::shared_ptr<const uint8_t*> keyboardState;
    CommandCenter commandCenter;
    uint8_t playerAmount;
    AudioEngine audioEngine;
    LobbyAction client_action;
    WrapperLobbyApp app;
    LobbyWindow lobbyWindow;
    QProcess process;


public:
    GameClient(const int window_width, const int window_height, const std::string& window_title,
               const int max_chunk_size_audio, const std::string& server_ip,
               const std::string& port, int& argc, char** argv);
    ~GameClient();
    void run();
    void receive_platforms_first_time();
    void run_lobby();
    int updateDuckStates();
    void input_thread();
    bool round_finished();
    void mainLoop(const int it);
    int exec_map_editor();
};

#endif
