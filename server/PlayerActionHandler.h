#ifndef PLAYER_ACTION_HANDLER_H
#define PLAYER_ACTION_HANDLER_H

#include <functional>
#include <map>
#include <memory>
#include <unordered_map>
#include <utility>

#include <SDL2/SDL.h>

#include "../common/common_queue.h"
#include "../common/duck.h"
#include "../common/types/action_type.h"

#include "server_gameloop_message.h"

// bien hecho. esto es lo que se conoce como table methods. como alternativa podrían haber hecho un patron command
class PlayerActionHandler {

private:
    std::map<std::uint8_t, std::function<void(Duck&)>> action_handlers;
    std::unordered_map<uint8_t, std::unique_ptr<Duck>>& ducks;
    std::map<std::uint8_t, void (PlayerActionHandler::*)(Duck& duck)> action_to_handlers = {
            {MOVE_RIGHT_KEY_DOWN, &PlayerActionHandler::handle_move_right_key_down},
            {MOVE_RIGHT_KEY_UP, &PlayerActionHandler::handle_move_right_key_up},
            {MOVE_LEFT_KEY_DOWN, &PlayerActionHandler::handle_move_left_key_down},
            {MOVE_LEFT_KEY_UP, &PlayerActionHandler::handle_move_left_key_up},
            {JUMP_KEY_DOWN, &PlayerActionHandler::handle_jump_key_down},
            {JUMP_KEY_UP, &PlayerActionHandler::handle_jump_key_up},
            {SHOOT_KEY_DOWN, &PlayerActionHandler::handle_shoot_key_down},
            {SHOOT_KEY_UP, &PlayerActionHandler::handle_shoot_key_up},
            {LOOKING_RIGHT_KEY_DOWN, &PlayerActionHandler::handle_looking_right_key_down},
            {LOOKING_LEFT_KEY_DOWN, &PlayerActionHandler::handle_looking_left_key_down},
            {LOOKING_UP_KEY_DOWN, &PlayerActionHandler::handle_looking_up_key_down},
            {LOOKING_DOWN_KEY_DOWN, &PlayerActionHandler::handle_looking_down_key_down},
            {LOOKING_DOWN_KEY_UP, &PlayerActionHandler::handle_looking_down_key_up},
            {LOOKING_UP_KEY_UP, &PlayerActionHandler::handle_looking_up_key_up}};

    void add_handler(std::uint8_t action, void (PlayerActionHandler::*handler)(Duck& duck));
    void init_handler_map();
    void init_handlers();
    void handle_move_right_key_down(Duck& duck);
    void handle_move_right_key_up(Duck& duck);
    void handle_move_left_key_down(Duck& duck);
    void handle_move_left_key_up(Duck& duck);
    void handle_jump_key_down(Duck& duck);
    void handle_jump_key_up(Duck& duck);
    void handle_shoot_key_down(Duck& duck);
    void handle_shoot_key_up(Duck& duck);
    void handle_looking_right_key_down(Duck& duck);
    void handle_looking_left_key_down(Duck& duck);
    void handle_looking_up_key_down(Duck& duck);
    void handle_looking_down_key_down(Duck& duck);
    void handle_looking_down_key_up(Duck& duck);
    void handle_looking_up_key_up(Duck& duck);

public:
    explicit PlayerActionHandler(std::unordered_map<uint8_t, std::unique_ptr<Duck>>& ducks);

    void process_player_action(const GameloopMessage& msg);
    ~PlayerActionHandler();
};


#endif  // PLAYER_ACTION_HANDLER_H
