#include "PlayerActionHandler.h"

#include <unordered_map>
#include <utility>

// #include "../common/types/action_type.h"
#include "server_gameloop_message.h"

enum Directions : const uint8_t { LEFT, RIGHT, UP, DOWN };

PlayerActionHandler::PlayerActionHandler(std::unordered_map<uint8_t, std::unique_ptr<Duck>>& ducks):
        ducks(ducks) {
    init_handlers();
}


void PlayerActionHandler::init_handlers() {

    for (const auto& action: action_to_handlers) {
        add_handler(action.first, action.second);
    }
}

void PlayerActionHandler::add_handler(std::uint8_t player_action,
                                      void (PlayerActionHandler::*handler)(Duck& duck)) {
    action_handlers.emplace(player_action, std::bind(handler, this, std::placeholders::_1));
}
#include <iostream>

void PlayerActionHandler::process_player_action(const GameloopMessage& msg) {

    auto it = ducks.find(msg.player_id);
    if (it == ducks.end())
        return;

    Duck* duck = it->second.get();
    if (!duck->is_alive) {
        return;
    }

    // action_handlers.at(key)(*this);
    // No se si hacerlo así para evitar las excepciones
    auto it_2 = action_handlers.find(msg.action);
    if (it_2 != action_handlers.end()) {
        auto handler = it_2->second;
        handler(*duck);
    }
}

void PlayerActionHandler::handle_move_right_key_down(Duck& duck) { duck.move_to(RIGHT); }

void PlayerActionHandler::handle_move_right_key_up(Duck& duck) { duck.stop_running(); }

void PlayerActionHandler::handle_move_left_key_down(Duck& duck) { duck.move_to(LEFT); }

void PlayerActionHandler::handle_move_left_key_up(Duck& duck) { duck.stop_running(); }

void PlayerActionHandler::handle_jump_key_down(Duck& duck) { duck.jump(true); }

void PlayerActionHandler::handle_jump_key_up(Duck& duck) { duck.jump(false); }

void PlayerActionHandler::handle_shoot_key_down(Duck& duck) { duck.shoot(true); }

void PlayerActionHandler::handle_shoot_key_up(Duck& duck) { duck.shoot(false); }

void PlayerActionHandler::handle_looking_right_key_down(Duck& duck) { duck.look_to(RIGHT); }

void PlayerActionHandler::handle_looking_left_key_down(Duck& duck) { duck.look_to(LEFT); }

void PlayerActionHandler::handle_looking_up_key_down(Duck& duck) { duck.look_to(UP); }

void PlayerActionHandler::handle_looking_down_key_down(Duck& duck) { duck.duck(true); }

void PlayerActionHandler::handle_looking_down_key_up(Duck& duck) { duck.duck(false); }

void PlayerActionHandler::handle_looking_up_key_up(Duck& duck) { duck.look_to(UP); }

PlayerActionHandler::~PlayerActionHandler() {}
