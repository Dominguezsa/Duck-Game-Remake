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

void ClientProtocol::read_msg(void* msg) {

    uint8_t duck_amount;
    recv_uint_8(duck_amount);

    // std::cout << "I received this many ducks: " << std::hex << +duck_amount << std::endl;

    std::vector<DuckState>* ducks = static_cast<std::vector<DuckState>*>(msg);

    uint8_t duck_id;
    uint32_t x;
    uint32_t y;
    uint8_t is_alive;
    uint8_t is_running;
    uint8_t is_jumping;
    uint8_t is_ducking;
    uint8_t is_shooting;
    uint8_t helmet_on;
    uint8_t armor_on;
    uint8_t weapon;

    for (int i = 0; i < duck_amount; i++) {
        recv_uint_8(duck_id);
        // std::cout << "For duck " << +duck_id << std::endl;
        recv_uint_32(x);
        // std::cout << "x: " << x << std::endl;
        recv_uint_32(y);
        // std::cout << "y: " << y << std::endl;
        recv_uint_8(is_alive);
        recv_uint_8(is_running);
        recv_uint_8(is_jumping);
        recv_uint_8(is_ducking);
        recv_uint_8(is_shooting);
        recv_uint_8(helmet_on);
        recv_uint_8(armor_on);
        recv_uint_8(weapon);

        DuckState duck_state(duck_id, Position(x, y), is_alive, is_running, is_jumping, is_ducking,
                             is_shooting, helmet_on, armor_on, WeaponType(weapon));
        ducks->push_back(duck_state);
    }

    // std::cout << "I received everything in theory\n";

    // std::cout << msg << std::endl;
}

void ClientProtocol::send_msg(void* msg) {
    uint8_t message = *static_cast<uint8_t*>(msg);
    // int int_msg = *static_cast<int*>(msg);
    send_data(&message, sizeof(uint8_t));
    std::cout << "Sending message: " << std::hex << +message << std::endl;
}
