#ifndef DUCK_STATE_H
#define DUCK_STATE_H

#include <cstdint>

#include "../common_position.h"

#include "weapon_type.h"

struct DuckState {
    uint8_t duck_id;  // protocol class is who assigns the id
    uint8_t life_points;
    uint8_t looking;  // 0 -> left, 1 -> right, 2 -> up, 3 -> down
    Position position;
    uint8_t is_alive;  // 1: true, 0: false
    uint8_t is_running;
    uint8_t is_jumping;
    uint8_t is_gliding;
    uint8_t is_falling;
    uint8_t is_ducking;
    uint8_t is_shooting;
    uint8_t helmet_on;
    uint8_t armor_on;
    uint8_t in_air;
    float vertical_velocity;
    WeaponType weapon;

    // Constructor
    DuckState(uint8_t _id, uint8_t starting_life_points, uint8_t looking, Position _position,
              uint8_t _is_alive, uint8_t _is_running, uint8_t _is_jumping, uint8_t _is_gliding,
              uint8_t _is_falling, uint8_t _is_ducking, uint8_t _is_shooting, uint8_t _helmet_on,
              uint8_t _armor_on, uint8_t _in_air, float _vertical_velocity, WeaponType _weapon):
            duck_id(_id),
            life_points(starting_life_points),
            looking(looking),
            position(_position),
            is_alive(_is_alive),
            is_running(_is_running),
            is_jumping(_is_jumping),
            is_gliding(_is_gliding),
            is_falling(_is_falling),
            is_ducking(_is_ducking),
            is_shooting(_is_shooting),
            helmet_on(_helmet_on),
            armor_on(_armor_on),
            in_air(_in_air),
            vertical_velocity(_vertical_velocity),
            weapon(_weapon) {}

    // Generic empty constructor
    DuckState():
            duck_id(0),
            life_points(100),
            looking(0),
            position(Position(0, 0)),
            is_alive(0),
            is_running(0),
            is_jumping(0),
            is_gliding(0),
            is_falling(0),
            is_ducking(0),
            is_shooting(0),
            helmet_on(0),
            armor_on(0),
            in_air(0),
            vertical_velocity(0.0f),
            weapon(WeaponType::NoneType) {}
};

#endif  // DUCK_STATE_H
