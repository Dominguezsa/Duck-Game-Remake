#include "protocol.h"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "../common/common_position.h"
#include "../common/duck.h"
#include "../common/snapshot.h"
#include "../common/types/duck_state.h"
#include "../common/types/match_state.h"
#include "../common/types/weapon_type.h"

ClientProtocol::ClientProtocol(Socket& socket): Protocol(socket) {}

void ClientProtocol::read_msg(void* msg) {
    uint8_t duck_amount;
    recv_uint_8(duck_amount);

    Snapshot* snapsho = static_cast<Snapshot*>(msg);
    std::string name;
    uint8_t duck_id;
    uint8_t life_points;
    uint8_t looking;
    uint32_t x;
    uint32_t y;
    uint8_t is_alive;
    uint8_t is_running;
    uint8_t is_jumping;
    uint8_t is_gliding;
    uint8_t is_falling;
    uint8_t is_ducking;
    uint8_t is_shooting;
    uint8_t is_sliding;
    uint8_t helmet_on;
    uint8_t armor_on;
    uint8_t in_air;
    float vertical_velocity;
    float horizontal_velocity;
    uint8_t weaponType;

    for (int i = 0; i < duck_amount; i++) {
        recv_string(name);
        recv_uint_8(duck_id);
        recv_uint_8(life_points);
        recv_uint_8(looking);
        recv_uint_32(x);
        recv_uint_32(y);
        recv_uint_8(is_alive);
        recv_uint_8(is_running);
        recv_uint_8(is_jumping);
        recv_uint_8(is_gliding);
        recv_uint_8(is_falling);
        recv_uint_8(is_ducking);
        recv_uint_8(is_shooting);
        recv_uint_8(is_sliding);
        recv_uint_8(helmet_on);
        recv_uint_8(armor_on);
        recv_uint_8(in_air);
        recv_float(vertical_velocity);
        recv_float(horizontal_velocity);
        recv_uint_8(weaponType);

        DuckState duck_state(name, duck_id, life_points, looking, Position(x, y), is_alive,
                             is_running, is_jumping, is_gliding, is_falling, is_ducking,
                             is_shooting, is_sliding, helmet_on, armor_on, in_air,
                             vertical_velocity, horizontal_velocity, WeaponType(weaponType));

        snapsho->ducks.push_back(duck_state);
    }

    uint8_t bullet_amount;
    recv_uint_8(bullet_amount);
    for (int i = 0; i < bullet_amount; i++) {
        uint8_t bullet_id;
        uint32_t bullet_x;
        uint32_t bullet_y;
        uint8_t going_right;
        uint8_t angle;
        recv_uint_8(bullet_id);
        recv_uint_32(bullet_x);
        recv_uint_32(bullet_y);
        recv_uint_8(going_right);
        recv_uint_8(angle);

        int true_angle = (int)angle - 128;


        Bullet bullet(bullet_id, bullet_x, bullet_y, static_cast<bool>(going_right), true_angle);
        snapsho->bullets.push_back(bullet);
    }


    uint8_t weapon_amount;

    recv_uint_8(weapon_amount);

    for (int i = 0; i < weapon_amount; i++) {
        uint8_t weapon_id;
        uint32_t weapon_x;
        uint32_t weapon_y;
        recv_uint_8(weapon_id);
        recv_uint_32(weapon_x);
        recv_uint_32(weapon_y);
        Weapon weapon(
                weapon_id,
                Position(weapon_x,
                         weapon_y));
        snapsho->weapons.push_back(weapon);
    }
}

void ClientProtocol::send_msg(void* msg) {
    uint8_t message = *static_cast<uint8_t*>(msg);
    send_data(&message, sizeof(uint8_t));
}

void ClientProtocol::recv_platforms(std::vector<Platform>& platforms) {
    uint8_t platform_amount;
    recv_uint_8(platform_amount);
    for (int i = 0; i < platform_amount; i++) {
        uint32_t x;
        uint32_t y;
        uint32_t width;
        uint32_t height;
        uint8_t platform_id;

        recv_uint_32(x);
        recv_uint_32(y);
        recv_uint_32(width);
        recv_uint_32(height);
        recv_uint_8(platform_id);

        Platform platform;
        platform.x = x;
        platform.y = y;
        platform.width = width;
        platform.height = height;
        platform.id = platform_id;
        platforms.push_back(platform);
    }
}
