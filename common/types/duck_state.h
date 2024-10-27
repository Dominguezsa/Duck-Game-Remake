#ifndef DUCK_STATE_H
#define DUCK_STATE_H

#include <cstdint>
#include "weapon_type.h"
#include "../common_position.h"

struct DuckState {
    uint8_t duck_id; // protocol class is who assigns the id
    Position position;
    uint8_t is_alive;  // 1: true, 0: false
    uint8_t is_running;
    uint8_t is_jumping;
    uint8_t is_ducking;
    uint8_t is_shooting;
    uint8_t helmet_on;
    uint8_t armor_on;
    WeaponType weapon; 

    // Constructor
    DuckState(uint8_t _id, Position _position,
              uint8_t _is_alive, uint8_t _is_running,
              uint8_t _is_jumping, uint8_t _is_ducking,
              uint8_t _is_shooting, uint8_t _helmet_on,
              uint8_t _armor_on, WeaponType _weapon) 
              :
              duck_id(_id), position(_position), is_alive(_is_alive),
              is_running(_is_running), is_jumping(_is_jumping),
              is_ducking(_is_ducking), is_shooting(_is_shooting),
              helmet_on(_helmet_on), armor_on(_armor_on),
              weapon(_weapon) {}
};

#endif  // DUCK_STATE_H
