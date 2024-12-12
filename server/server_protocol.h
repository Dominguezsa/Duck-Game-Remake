#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <cstdint>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "../common/common_protocol.h"
#include "../common/common_socket.h"
#include "../common/snapshot.h"
#include "../common/types/duck_state.h"
#include "../common/types/match_state.h"

#include "server_gameloop_message.h"

class ServerProtocol: public Protocol {
public:
    explicit ServerProtocol(Socket& skt): Protocol(skt) {}

    void recv_duck_action(uint8_t& command);
    void send_snapshot(std::shared_ptr<Snapshot> snapshot);

    void read_msg(void* msg) override;  // No se usa
    void send_msg(void* msg) override;  // No se usa

    void send_game_map_list(const std::list<std::string>& map_names);


    void recv_match_info(std::string& map_name, std::string& match_name,
                         uint8_t& number_of_players);
    void send_confirmation(bool success);

    void send_match_list(const std::list<std::string>& match_names);

    void recv_lobby_action(char& action);
};

#endif  // SERVER_PROTOCOLO_H
