#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

#include <string>

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

#include "ResourceManager.h"

class GameClient {
private:
    SDL2pp::SDL sdl;
    SDL2pp::SDLTTF ttf;
    SDL2pp::Mixer mixer;
    SDL2pp::Window window;
    SDL2pp::Renderer renderer;
    ResourceManager resourceManager;

public:
    GameClient(const int window_width, const int window_height, const std::string& window_title,
               const int max_chunk_size_audio);
    ~GameClient();
    void run();
    void mainLoop(const int it, bool& quit);
};

#endif
