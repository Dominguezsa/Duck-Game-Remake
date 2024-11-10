#include "CommandCenter.h"

#include <memory>
#include <utility>

#include <SDL2/SDL_events.h>

#include "../common/types/action_type.h"

CommandCenter::CommandCenter(Queue<uint8_t>& messagesForServer,
                             std::shared_ptr<const uint8_t*> keyboardState, bool& quit):
        messagesForServer(messagesForServer), keyboardState(keyboardState), quit(quit) {

    init_handlers();
}


void CommandCenter::init_handlers() {
    // Ugly, but it works

    add_handler(SDL_QUIT, SDLK_UNKNOWN, &CommandCenter::handle_quit);
    add_handler(SDL_KEYDOWN, SDLK_ESCAPE, &CommandCenter::handle_key_down_escape);
    add_handler(SDL_KEYDOWN, SDLK_d, &CommandCenter::handle_key_down_d);
    add_handler(SDL_KEYDOWN, SDLK_a, &CommandCenter::handle_key_down_a);
    add_handler(SDL_KEYDOWN, SDLK_w, &CommandCenter::handle_key_down_w);
    add_handler(SDL_KEYDOWN, SDLK_s, &CommandCenter::handle_key_down_s);
    add_handler(SDL_KEYDOWN, SDLK_SPACE, &CommandCenter::handle_key_down_space);
    add_handler(SDL_KEYDOWN, SDLK_f, &CommandCenter::handle_key_down_f);
    add_handler(SDL_KEYUP, SDLK_d, &CommandCenter::handle_key_up_d);
    add_handler(SDL_KEYUP, SDLK_a, &CommandCenter::handle_key_up_a);
    add_handler(SDL_KEYUP, SDLK_w, &CommandCenter::handle_key_up_w);
    add_handler(SDL_KEYUP, SDLK_s, &CommandCenter::handle_key_up_s);
    add_handler(SDL_KEYUP, SDLK_SPACE, &CommandCenter::handle_key_up_space);
    add_handler(SDL_KEYUP, SDLK_f, &CommandCenter::handle_key_up_f);
}

void CommandCenter::add_handler(SDL_EventType event_type, SDL_Keycode key,
                                void (CommandCenter::*handler)()) {
    event_handlers.emplace(std::make_pair(event_type, key), std::bind(handler, this));
}

void CommandCenter::processEvent(const SDL_Event& event) {

    auto key = std::make_pair(static_cast<SDL_EventType>(event.type), event.key.keysym.sym);
    try {
        event_handlers.at(key)(*this);
        // No se si hacerlo así para evitar las excepciones
        // auto it = event_handlers.find(key);
        // if (it != event_handlers.end()) {
        //     auto handler = it->second;
        //     handler(*this);
        // }

    } catch (const std::out_of_range& e) {
        // Do nothing
        // std::cout << "Trying to handle an event thats not in the map" << std::endl;
    }
}

void CommandCenter::handle_quit() { quit = true; }

void CommandCenter::handle_key_down_escape() { quit = true; }

void CommandCenter::handle_key_down_d() { messagesForServer.push(MOVE_RIGHT_KEY_DOWN); }

void CommandCenter::handle_key_down_a() { messagesForServer.push(MOVE_LEFT_KEY_DOWN); }

void CommandCenter::handle_key_down_w() { messagesForServer.push(LOOKING_UP_KEY_DOWN); }

void CommandCenter::handle_key_down_s() { messagesForServer.push(LOOKING_DOWN_KEY_DOWN); }

void CommandCenter::handle_key_down_space() { messagesForServer.push(JUMP_KEY_DOWN); }

void CommandCenter::handle_key_down_f() { messagesForServer.push(SHOOT_KEY_DOWN); }

void CommandCenter::handle_key_up_d() {
    if ((*keyboardState)[SDL_SCANCODE_A]) {
        messagesForServer.push(LOOKING_LEFT_KEY_DOWN);
        return;
    }
    messagesForServer.push(MOVE_RIGHT_KEY_UP);
}

void CommandCenter::handle_key_up_a() {
    if ((*keyboardState)[SDL_SCANCODE_D]) {
        messagesForServer.push(LOOKING_RIGHT_KEY_DOWN);
        return;
    }
    messagesForServer.push(MOVE_LEFT_KEY_UP);
}

void CommandCenter::handle_key_up_w() { messagesForServer.push(LOOKING_UP_KEY_UP); }

void CommandCenter::handle_key_up_s() { messagesForServer.push(LOOKING_DOWN_KEY_UP); }

void CommandCenter::handle_key_up_space() { messagesForServer.push(JUMP_KEY_UP); }

void CommandCenter::handle_key_up_f() { messagesForServer.push(SHOOT_KEY_UP); }

CommandCenter::~CommandCenter() {}
