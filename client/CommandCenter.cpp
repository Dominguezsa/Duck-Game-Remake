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

    for (const auto& event: event_to_handlers) {
        add_handler(event.first, event.second);
    }
}

void CommandCenter::add_handler(std::pair<SDL_EventType, SDL_Keycode> key,
                                void (CommandCenter::*handler)()) {
    event_handlers.emplace(key, std::bind(handler, this));
}

void CommandCenter::processEvent(const SDL_Event& event) {

    auto key = std::make_pair(static_cast<SDL_EventType>(event.type), event.key.keysym.sym);
    auto it = event_handlers.find(key);
    if (it != event_handlers.end()) {
        auto handler = it->second;
        handler(*this);
    }
}

void CommandCenter::handle_quit() { quit = true; }

void CommandCenter::handle_key_down_escape() { quit = true; }

void CommandCenter::handle_key_down_d() { messagesForServer.push(MOVE_RIGHT_KEY_DOWN); }

void CommandCenter::handle_key_down_a() { messagesForServer.push(MOVE_LEFT_KEY_DOWN); }

void CommandCenter::handle_key_down_w() {
    messagesForServer.push(LOOKING_UP_KEY_DOWN);
}

void CommandCenter::handle_key_down_s() {
    if ((*keyboardState)[SDL_SCANCODE_SPACE]) {
        return;
    }
    messagesForServer.push(LOOKING_DOWN_KEY_DOWN);
}

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

void CommandCenter::handle_key_up_w() {
    messagesForServer.push(LOOKING_UP_KEY_UP);
}

void CommandCenter::handle_key_up_s() {
    messagesForServer.push(LOOKING_DOWN_KEY_UP);
    if ((*keyboardState)[SDL_SCANCODE_A]) {
        messagesForServer.push(MOVE_LEFT_KEY_DOWN);
    } else if ((*keyboardState)[SDL_SCANCODE_D]) {
        messagesForServer.push(MOVE_RIGHT_KEY_DOWN);
    }
}

void CommandCenter::handle_key_up_space() { messagesForServer.push(JUMP_KEY_UP); }

void CommandCenter::handle_key_up_f() { messagesForServer.push(SHOOT_KEY_UP); }

CommandCenter::~CommandCenter() {}
