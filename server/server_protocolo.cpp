#include "server_protocolo.h"

#include <stdexcept>

#include <netinet/in.h>

void ServerProtocol::recv_msg(GameloopMessage& msg) {
    recv_uint_8(msg.action);
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

void ServerProtocol::apagarSocket() {
    this->skt.shutdown(2);
    this->skt.close();
}
