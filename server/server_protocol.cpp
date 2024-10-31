#include "server_protocol.h"

void ServerProtocol::recv_msg(uint8_t& command) { recv_uint_8(command); }

void ServerProtocol::send_duck_states(std::shared_ptr<std::vector<DuckState>> states) {
    // Refactorizable!
    // std::cout << "I entered the send_duck_states method\n";
    unsigned int state_count = states->size();
    send_data(&state_count, sizeof(uint8_t));
    std::vector<DuckState> state_vector = *states;
    for (uint16_t i = 0; i < state_count; i++) {
        send_data(&state_vector[i].duck_id, sizeof(uint8_t));
        send_data(&state_vector[i].position.x, sizeof(uint32_t));
        // std::cout << "state_vector[i].position.x: " << state_vector[i].position.x << std::endl;
        send_data(&state_vector[i].position.y, sizeof(uint32_t));
        // std::cout << "state_vector[i].position.y: " << state_vector[i].position.y << std::endl;
        send_data(&state_vector[i].is_alive, sizeof(uint8_t));
        send_data(&state_vector[i].is_running, sizeof(uint8_t));
        send_data(&state_vector[i].is_jumping, sizeof(u_int8_t));
        send_data(&state_vector[i].is_ducking, sizeof(uint8_t));
        send_data(&state_vector[i].is_shooting, sizeof(uint8_t));
        send_data(&state_vector[i].helmet_on, sizeof(uint8_t));
        send_data(&state_vector[i].armor_on, sizeof(uint8_t));
        send_data(&state_vector[i].weapon, sizeof(uint8_t));

        // std::cout << "Sended the data regarding duck " << +state_vector[i].duck_id << std::endl;
    }
}

// Solo para compilar, esto no se usa se tiene que ir:

void ServerProtocol::read_msg(void* msg) { std::cout << msg << std::endl; }

void ServerProtocol::send_msg(void* msg) {
    int int_msg = *static_cast<int*>(msg);
    std::cout << "Sending message: " << int_msg << std::endl;
}
