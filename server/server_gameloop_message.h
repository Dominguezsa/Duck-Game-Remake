#ifndef SERVER_GAMELOOP_MSG_H
#define SERVER_GAMELOOP_MSG_H

#include <cstdint>
#include "../common/types/action_type.h"

struct GameloopMessage {
    uint8_t player_id;
    uint8_t action;
    uint8_t data;  // Additional data (e.g., weapon type, direction)

    GameloopMessage() : player_id(0), action(0), data(0) {}
    
    GameloopMessage(uint8_t id, uint8_t _action, uint8_t _data = 0) 
        : player_id(id), action(_action), data(_data) {}
};

#endif // SERVER_GAMELOOP_MSG_H
