#ifndef COMMAND_CENTER_H
#define COMMAND_CENTER_H

#include <functional>
#include <map>
#include <memory>
#include <utility>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2pp/SDL2pp.hh>

#include "../common/common_queue.h"

#include "ResourceManager.h"

class CommandCenter {

private:
    std::map<std::pair<SDL_EventType, SDL_Keycode>, std::function<void(CommandCenter&)>>
            event_handlers;
    Queue<uint8_t>& messagesForServer;
    std::shared_ptr<const uint8_t*> keyboardState;
    bool& quit;
    std::map<std::pair<SDL_EventType, SDL_Keycode>, void (CommandCenter::*)()> event_to_handlers = {
            {std::make_pair(SDL_QUIT, SDLK_UNKNOWN), &CommandCenter::handle_quit},
            {std::make_pair(SDL_KEYDOWN, SDLK_ESCAPE), &CommandCenter::handle_key_down_escape},
            {std::make_pair(SDL_KEYDOWN, SDLK_d), &CommandCenter::handle_key_down_d},
            {std::make_pair(SDL_KEYDOWN, SDLK_a), &CommandCenter::handle_key_down_a},
            {std::make_pair(SDL_KEYDOWN, SDLK_w), &CommandCenter::handle_key_down_w},
            {std::make_pair(SDL_KEYDOWN, SDLK_s), &CommandCenter::handle_key_down_s},
            {std::make_pair(SDL_KEYDOWN, SDLK_SPACE), &CommandCenter::handle_key_down_space},
            {std::make_pair(SDL_KEYDOWN, SDLK_f), &CommandCenter::handle_key_down_f},
            {std::make_pair(SDL_KEYUP, SDLK_d), &CommandCenter::handle_key_up_d},
            {std::make_pair(SDL_KEYUP, SDLK_a), &CommandCenter::handle_key_up_a},
            {std::make_pair(SDL_KEYUP, SDLK_w), &CommandCenter::handle_key_up_w},
            {std::make_pair(SDL_KEYUP, SDLK_s), &CommandCenter::handle_key_up_s},
            {std::make_pair(SDL_KEYUP, SDLK_SPACE), &CommandCenter::handle_key_up_space},
            {std::make_pair(SDL_KEYUP, SDLK_f), &CommandCenter::handle_key_up_f}};

public:
    CommandCenter(Queue<uint8_t>& messagesForServer, std::shared_ptr<const uint8_t*> keyboardState,
                  bool& quit);
    void add_handler(std::pair<SDL_EventType, SDL_Keycode>, void (CommandCenter::*handler)());
    void init_handlers();
    void handle_quit();
    void handle_key_down_escape();
    void handle_key_down_d();
    void handle_key_down_a();
    void handle_key_down_w();
    void handle_key_down_s();
    void handle_key_down_space();
    void handle_key_down_f();
    void handle_key_up_d();
    void handle_key_up_a();
    void handle_key_up_w();
    void handle_key_up_s();
    void handle_key_up_space();
    void handle_key_up_f();
    void processEvent(const SDL_Event& event);
    ~CommandCenter();
};


#endif  // COMMAND_CENTER_H
