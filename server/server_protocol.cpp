#include "server_protocol.h"

void ServerProtocol::recv_msg(uint8_t& command) { recv_uint_8(command); }


void ServerProtocol::send_snapshot(std::shared_ptr<Snapshot> snapshot) {
    unsigned int state_count = snapshot->ducks.size();
    send_data(&state_count, sizeof(uint8_t));
    uint32_t x;
    uint32_t y;
    std::vector<DuckState> duck_vector = snapshot->ducks;
    for (uint16_t i = 0; i < state_count; i++) {
        send_data(&duck_vector[i].duck_id, sizeof(uint8_t));
        send_data(&duck_vector[i].life_points, sizeof(uint8_t));
        send_data(&duck_vector[i].looking, sizeof(uint8_t));

        x = htonl(duck_vector[i].position.x);
        y = htonl(duck_vector[i].position.y);

        send_data(&x, sizeof(uint32_t));
        send_data(&y, sizeof(uint32_t));
        send_data(&duck_vector[i].is_alive, sizeof(uint8_t));
        send_data(&duck_vector[i].is_running, sizeof(uint8_t));
        send_data(&duck_vector[i].is_jumping, sizeof(u_int8_t));
        send_data(&duck_vector[i].is_gliding, sizeof(uint8_t));
        send_data(&duck_vector[i].is_falling, sizeof(uint8_t));
        send_data(&duck_vector[i].is_ducking, sizeof(uint8_t));
        send_data(&duck_vector[i].is_shooting, sizeof(uint8_t));
        send_data(&duck_vector[i].helmet_on, sizeof(uint8_t));
        send_data(&duck_vector[i].armor_on, sizeof(uint8_t));
        send_data(&duck_vector[i].in_air, sizeof(uint8_t));
        send_data(&duck_vector[i].vertical_velocity, sizeof(float));
        send_data(&duck_vector[i].weapon, sizeof(uint8_t));
    }

    unsigned int item_count = snapshot->bullets.size();
    send_data(&item_count, sizeof(uint8_t));
}
// Solo para compilar, esto no se usa se tiene que ir:

void ServerProtocol::read_msg(void* msg) { std::cout << msg << std::endl; }

void ServerProtocol::send_msg(void* msg) {
    int int_msg = *static_cast<int*>(msg);
    std::cout << "Sending message: " << int_msg << std::endl;
}
