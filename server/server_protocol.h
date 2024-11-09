#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <cstdint>
#include <iostream>
#include <list>
#include <memory>
#include <vector>

#include "../common/common_protocol.h"
#include "../common/common_socket.h"
#include "../common/types/duck_state.h"

#include "server_gameloop_message.h"

class ServerProtocol: public Protocol {
public:
    explicit ServerProtocol(Socket& skt): Protocol(skt) {}

    void recv_msg(uint8_t& command);

    void send_duck_states(std::shared_ptr<std::vector<DuckState>> states);

    // No se usan! Quitar despues de common/common_protocol.h
    void read_msg(void* msg) override;

    void send_msg(void* msg) override;

    // --------- Pre-match messages (lobby) -------
    
    // Pos: Sends the names of the existing game maps to the client.
    void send_game_map_list(std::list<std::string>& map_names);

    /* Pos: Receives necessary information to create a match:
            
            - A name of a map (selected by the client)
            - A match name
            - The number of players that will participate in the match.
    */
    void recv_match_info(std::string& map_name, std::string& match_name,
                         uint8_t& number_of_players);

    // Pos: Sends true if a client action was successful, false otherwise.
    void send_confirmation(bool success);

    // Pos: Sends the names of the active matches that the client can join.
    void send_match_list(std::list<std::string>& match_names);

    void recv_action(char& action);

    void recv_match_name(std::string& match_name);

    void recv_player_name(std::string& player_name);
};

#endif  // SERVER_PROTOCOLO_H
