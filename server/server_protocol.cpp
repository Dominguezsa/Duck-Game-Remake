#include "server_protocol.h"

void ServerProtocol::recv_msg(uint8_t &command) {
    recv_uint_8(command);
}

void ServerProtocol::send_duck_states(std::vector<DuckState>& states) {
    // Refactorizable!
    unsigned int state_count = states.size();
    send_data(&state_count, sizeof(uint8_t));
    for (uint16_t i = 0; i < state_count; i++) {
        send_data(&states[i].duck_id, sizeof(uint8_t));
        send_data(&states[i].position.x, sizeof(uint32_t));
        send_data(&states[i].position.y, sizeof(uint32_t));
        send_data(&states[i].is_alive, sizeof(uint8_t));
        send_data(&states[i].is_running, sizeof(uint8_t));
        send_data(&states[i].is_jumping, sizeof(u_int8_t));
        send_data(&states[i].is_ducking, sizeof(uint8_t));
        send_data(&states[i].is_shooting, sizeof(uint8_t));
        send_data(&states[i].helmet_on, sizeof(uint8_t));
        send_data(&states[i].armor_on, sizeof(uint8_t));
        send_data(&states[i].weapon, sizeof(uint8_t));
    }
}

// Solo para compilar, esto no se usa se tiene que ir:

void ServerProtocol::read_msg(void* msg) { std::cout << msg << std::endl; }

void ServerProtocol::send_msg(void* msg) {
    int int_msg = *static_cast<int*>(msg);
    std::cout << "Sending message: " << int_msg << std::endl;
}
