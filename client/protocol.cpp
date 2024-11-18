#include "protocol.h"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "../common/common_position.h"
#include "../common/duck.h"
#include "../common/types/duck_state.h"
#include "../common/types/weapon_type.h"

ClientProtocol::ClientProtocol(Socket& socket): Protocol(socket) {}

std::vector<Duck> ClientProtocol::receiveMessage() {
    // Placeholder
    std::vector<Duck> ducks;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return ducks;
}

void ClientProtocol::recv_player_amount(uint8_t& player_amount) { recv_uint_8(player_amount); }

void ClientProtocol::read_msg(void* msg) {
    uint8_t duck_amount;
    recv_uint_8(duck_amount);
    // std::cout << "In my read_msg function, duck_amount is: " << +duck_amount << std::endl;

    std::vector<DuckState>* ducks = static_cast<std::vector<DuckState>*>(msg);
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
    uint8_t helmet_on;
    uint8_t armor_on;
    uint8_t in_air;
    float vertical_velocity;
    uint8_t weapon;

    for (int i = 0; i < duck_amount; i++) {
        recv_string(name);
        recv_uint_8(duck_id);
        recv_uint_8(life_points);
        recv_uint_8(looking);
        // std::cout << "For duck " << +duck_id << std::endl;
        recv_uint_32(x);
        // std::cout << "x: " << +x << std::endl;
        recv_uint_32(y);
        // std::cout << "y: " << +y << std::endl;
        recv_uint_8(is_alive);
        recv_uint_8(is_running);
        recv_uint_8(is_jumping);
        recv_uint_8(is_gliding);
        recv_uint_8(is_falling);
        recv_uint_8(is_ducking);
        recv_uint_8(is_shooting);
        recv_uint_8(helmet_on);
        recv_uint_8(armor_on);
        recv_uint_8(in_air);
        recv_float(vertical_velocity);
        recv_uint_8(weapon);

        DuckState duck_state(name, duck_id, life_points, looking, Position(x, y), is_alive,
                             is_running, is_jumping, is_gliding, is_falling, is_ducking,
                             is_shooting, helmet_on, armor_on, in_air, vertical_velocity,
                             WeaponType(weapon));
        ducks->push_back(duck_state);
    }

    uint8_t bullet_amount;
    recv_uint_8(bullet_amount);
    // std::cout << +bullet_amount << std::endl;
    // std::cout << "I received everything in theory\n";

    // std::cout << msg << std::endl;
}

void ClientProtocol::send_msg(void* msg) {
    uint8_t message = *static_cast<uint8_t*>(msg);
    // int int_msg = *static_cast<int*>(msg);
    send_data(&message, sizeof(uint8_t));
    // std::cout << "Sending message: " << std::hex << +message << std::endl;
}
