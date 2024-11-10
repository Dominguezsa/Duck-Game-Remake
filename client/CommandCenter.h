#ifndef COMMAND_CENTER_H
#define COMMAND_CENTER_H

#include <functional>
#include <map>
#include <memory>
#include <utility>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../common/common_queue.h"

class CommandCenter {

private:
    std::map<std::pair<SDL_EventType, SDL_Keycode>, std::function<void(CommandCenter&)>>
            // cppcheck-suppress unusedStructMember
            event_handlers;
    Queue<uint8_t>& messagesForServer;
    std::shared_ptr<const uint8_t*> keyboardState;
    // cppcheck-suppress unusedStructMember
    bool& quit;

public:
    CommandCenter(Queue<uint8_t>& messagesForServer, std::shared_ptr<const uint8_t*> keyboardState,
                  bool& quit);
    void add_handler(SDL_EventType event_type, SDL_Keycode key, void (CommandCenter::*handler)());
    void init_handlers();
    // Estos dos son medio especiales porque reciben algo, y los demás handlers no, así que ver
    // después que hacer con esto
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
