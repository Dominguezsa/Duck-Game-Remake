#include "server_client_session.h"

#include <utility>
ClientSession::ClientSession(Socket _skt, uint8_t _id, MatchesMonitor& monitor):
        id(_id),
        skt(std::move(_skt)),
        protocol(this->skt),
        matches_monitor(monitor),
        client_queue(CLIENT_QUEUE_SIZE) { }

uint8_t ClientSession::get_id() const { return id; }

void ClientSession::end_communication() {
    // We disconnect the player from the any match it was in.
    matches_monitor.disconnect_player(id);

    // We close the communication channel.
    protocol.end_communication();
}

// --------------- Lobby logic ---------------

// Estas constantes hay que pasarlas a algun .h en
// common/types/ (tanto aca como del lado del cliente).
static constexpr char EXIT = 'Q';
static constexpr char CMD_CREATE = 'C';
static constexpr char CMD_JOIN = 'J';
static constexpr uint8_t SUCCESS = 0x01;
static constexpr uint8_t FAILURE = 0x00;

void ClientSession::run_lobby_loop() {
    try {
        while (true) {
            char lobby_action;
            this->protocol.recv_action(lobby_action);

            if (lobby_action == EXIT) {
                // Finalizar comunicacion con el cliente.
                break;
            }
            bool executed = exec_lobby_action(lobby_action);
            if (executed) { break; }
        }
    } catch (SocketWasCLosedException& e) {
        // El cliente se desconecto.
    }
}

bool ClientSession::exec_lobby_action(char action) {
    bool executed = false;

    std::string player_name;
    this->protocol.recv_player_name(player_name);

    switch (action) {
        case CMD_CREATE: {
            // (2)
            // Lo correcto seria que los nombres de mapas
            // sean provistos por alguna otra entidad.
            std::list<std::string> map_list;
            map_list.push_back("Golden Grove Clash");
            
            protocol.send_game_map_list(map_list);
            
            // (3)
            uint8_t number_of_players;
            std::string map_name; // dato no utilizado aun.
            std::string match_name;
            this->protocol.recv_match_info(map_name, match_name,
                                           number_of_players);
            
            bool success = matches_monitor.create_match(match_name,
                           number_of_players, id, &client_queue);
            
            // (4)
            protocol.send_confirmation(success);
            // ---
            if (success) {
                // Generar un DuckIdentity y enviar.
                // ....
            }
            return success;
        }
        case CMD_JOIN: {
            // (2)
            std::list<std::string> available_matches =
                 matches_monitor.get_available_match_names();

            protocol.send_match_list(available_matches);
            
            // (3)
            std::string match_name;
            this->protocol.recv_match_name(match_name);

            // (4)
            bool joined = matches_monitor.join_match(match_name, id,
                                                     &client_queue);
            protocol.send_confirmation(joined);

            if (joined) {
                // Generar un DuckIdentity y enviar.
                // ....
            }
            return joined;
        }
    }
}
