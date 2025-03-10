#include "server_protocol.h"

#include "../common/snapshot.h"
void ServerProtocol::recv_duck_action(uint8_t& command) { recv_uint_8(command); }

void ServerProtocol::send_snapshot(std::shared_ptr<Snapshot> snapshot) {
    if (snapshot->first_message) {
        unsigned int platform_count = snapshot->platforms.size();
        send_data(&platform_count, sizeof(uint8_t));
        uint32_t x;
        uint32_t y;
        uint32_t width;
        uint32_t height;
        uint8_t id;
        for (const auto& platform: snapshot->platforms) {
            x = htonl(platform.x);
            y = htonl(platform.y);
            width = htonl(platform.width);
            height = htonl(platform.height);
            id = platform.id;

            send_data(&x, sizeof(uint32_t));
            send_data(&y, sizeof(uint32_t));
            send_data(&width, sizeof(uint32_t));
            send_data(&height, sizeof(uint32_t));
            send_data(&id, sizeof(uint8_t));
        }
        return;
    }
    unsigned int state_count = snapshot->ducks.size();
    send_data(&state_count, sizeof(uint8_t));
    uint32_t x;
    uint32_t y;
    std::vector<DuckState> duck_vector = snapshot->ducks;
    for (uint16_t i = 0; i < state_count; i++) {
        send_string(duck_vector[i].name);
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
        send_data(&duck_vector[i].is_sliding, sizeof(uint8_t));
        send_data(&duck_vector[i].helmet_on, sizeof(uint8_t));
        send_data(&duck_vector[i].armor_on, sizeof(uint8_t));
        send_data(&duck_vector[i].in_air, sizeof(uint8_t));
        send_data(&duck_vector[i].vertical_velocity, sizeof(float));
        send_data(&duck_vector[i].horizontal_velocity, sizeof(float));
        send_data(&duck_vector[i].weapon, sizeof(uint8_t));
    }

    unsigned int item_count = snapshot->bullets.size();
    send_data(&item_count, sizeof(uint8_t));


    for (const auto& bullet: snapshot->bullets) {
        send_data(&bullet.id, sizeof(uint8_t));
        x = htonl(bullet.x);
        y = htonl(bullet.y);
        send_data(&x, sizeof(uint32_t));
        send_data(&y, sizeof(uint32_t));
        send_data(&bullet.going_right, sizeof(uint8_t));
        uint8_t angle = (uint8_t)bullet.angle + 128;
        send_data(&angle, sizeof(uint8_t));
    }


    unsigned int weapon_count = snapshot->weapons.size();
    send_data(&weapon_count, sizeof(uint8_t));

    for (uint16_t i = 0; i < weapon_count; i++) {
        send_data(&snapshot->weapons[i].id, sizeof(uint8_t));
        x = htonl(snapshot->weapons[i].pos.x);
        y = htonl(snapshot->weapons[i].pos.y);
        send_data(&x, sizeof(uint32_t));
        send_data(&y, sizeof(uint32_t));
    }
}

void ServerProtocol::read_msg(void* msg) { std::cout << msg << std::endl; }

void ServerProtocol::send_msg(void* msg) {
    int int_msg = *static_cast<int*>(msg);
    std::cout << "Sending message: " << int_msg << std::endl;
}

// --------- Pre-match messages (lobby) -------

void ServerProtocol::send_game_map_list(const std::list<std::string>& map_names) {
    uint16_t map_count = map_names.size();
    map_count = htons(map_count);
    send_data(&map_count, sizeof(uint16_t));

    for (const std::string& map_name: map_names) {
        send_string(map_name);
    }
}

void ServerProtocol::recv_match_info(std::string& map_name, std::string& match_name,
                                     uint8_t& number_of_players) {
    uint8_t buf;
    recv_uint_8(buf);
    recv_uint_8(number_of_players);
    recv_string(match_name);
    recv_string(map_name);
}

void ServerProtocol::send_confirmation(bool success) { send_data(&success, sizeof(uint8_t)); }

void ServerProtocol::send_match_list(const std::list<std::string>& match_names) {
    uint16_t match_count = match_names.size();
    match_count = htons(match_count);
    send_data(&match_count, sizeof(uint16_t));

    for (const std::string& match_name: match_names) {
        send_string(match_name);
    }
}

void ServerProtocol::recv_lobby_action(char& action) {
    uint8_t buf;
    recv_uint_8(buf);
    action = buf;
}
