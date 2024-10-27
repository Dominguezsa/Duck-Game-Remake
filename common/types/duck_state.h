#ifndef DUCK_STATE_H
#define DUCK_STATE_H

#include <cstdint>
#include "weapon_type.h"
#include "../common_position.h"

struct DuckState {
    uint8_t duck_id; // ServerProtocol class is who assigns the id
    Position position; // Cord x and y are uint32_t (infoModelo)
    bool is_alive;
    bool is_running;
    bool is_jumping;
    bool is_ducking;
    bool is_shooting;
    bool helmet_on;
    bool armor_on;
    WeaponType weapon; 

    // Constructor
    DuckState(uint8_t _id, Position _position,
              bool _is_alive, bool _is_running,
              bool _is_jumping, bool _is_ducking,
              bool _is_shooting, bool _helmet_on,
              bool _armor_on, WeaponType _weapon) 
              :
              duck_id(_id), position(_position), is_alive(_is_alive),
              is_running(_is_running), is_jumping(_is_jumping),
              is_ducking(_is_ducking), is_shooting(_is_shooting),
              helmet_on(_helmet_on), armor_on(_armor_on),
              weapon(_weapon) {}
};

#endif  // DUCK_STATE_H
