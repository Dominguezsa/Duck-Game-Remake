#ifndef SERVER_GAMELOOP_MSG_H
#define SERVER_GAMELOOP_MSG_H

#include <cstdint>
#include "../common/types/action_type.h"

struct GameloopMessage {
    uint8_t player_id;
    uint8_t action;

    // Constructor
    GameloopMessage(uint8_t id, uint8_t _action):
    player_id(id), action(_action) {}
};

#endif  // SERVER_GAMELOOP_MSG_H
