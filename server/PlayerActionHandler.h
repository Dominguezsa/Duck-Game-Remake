#ifndef PLAYER_ACTION_HANDLER_H
#define PLAYER_ACTION_HANDLER_H

#include <functional>
#include <map>
#include <memory>
#include <unordered_map>
#include <utility>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../common/common_queue.h"
#include "../common/duck.h"
#include "../common/types/action_type.h"

#include "server_gameloop_message.h"

class PlayerActionHandler {

private:
    // cppcheck-suppress unusedStructMember
    std::map<std::uint8_t, std::function<void(Duck&)>> action_handlers;
    // cppcheck-suppress unusedStructMember
    std::unordered_map<uint8_t, std::unique_ptr<Duck>>& ducks;


    void add_handler(std::uint8_t action, void (PlayerActionHandler::*handler)(Duck& duck));
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

public:
    explicit PlayerActionHandler(std::unordered_map<uint8_t, std::unique_ptr<Duck>>& ducks);

    void process_player_action(const GameloopMessage& msg);
    ~PlayerActionHandler();
};


#endif  // PLAYER_ACTION_HANDLER_H
