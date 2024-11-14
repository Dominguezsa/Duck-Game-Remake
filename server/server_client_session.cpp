#include "server_client_session.h"

#include <utility>
ClientSession::ClientSession(Socket _skt, MatchesMonitor& monitor):
        identity(),
        skt(std::move(_skt)),
        protocol(this->skt),
        matches_monitor(monitor),
        client_queue(CLIENT_QUEUE_SIZE) {}

uint8_t ClientSession::get_id() const { return identity.id; }

void ClientSession::end_communication() {
    // We disconnect the player from the any match it was in.
    matches_monitor.disconnect_player(identity.joined_match_name, identity.id);

    // We close the communication channel.
    protocol.end_communication();
}

Queue<GameloopMessage>* ClientSession::get_match_queue() {
    return matches_monitor.get_match_queue(identity.joined_match_name);
}

void ClientSession::run_receiver_loop() {
    GameloopMessage msg;
    msg.player_id = this->identity.id;
    Queue<GameloopMessage>* gameloop_queue = get_match_queue();
    try {
        while (this->_is_alive) {
            std::cout << "Waiting for message\n";
            protocol.recv_msg(msg.action);
            gameloop_queue->push(msg);
        }
    } catch (const ClosedQueue& e) {
        this->matches_monitor.disconnect_player(identity.joined_match_name, identity.id);
    }
}

void ClientSession::run() {
    try {
        while (this->_is_alive) {
            run_lobby_loop();
            if (!this->_is_alive) {
                break;
            }

            // At this point, the client is in a match.
            SenderThread sender(protocol, client_queue);
            sender.start();
            run_receiver_loop();
            sender.stop();
            sender.join();
        }
    } catch (const SocketWasCLosedException& e) {
        this->_is_alive = false;
        end_communication();
    } catch (const std::exception& err) {
        this->_is_alive = false;
        end_communication();
    }
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
    bool success = false;
    try {
        while (true) {
            char lobby_action;
            this->protocol.recv_action(lobby_action);
            if (lobby_action == EXIT) {
                this->_is_alive = false;
                break;
            }
            exec_lobby_action(lobby_action, success);
            if (success) {
                break;
            }
        }
    } catch (SocketWasCLosedException& e) {
        if (success) {
            end_communication();
        }
        this->_is_alive = false;
    }
}

void ClientSession::exec_lobby_action(char action, bool& success) {
    std::string player_name, match_name;
    this->protocol.recv_player_name(player_name);
    DuckIdentity duck_info;
    identity.name = player_name;
    duck_info.name = player_name;

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
            std::string map_name;
            this->protocol.recv_match_info(map_name, match_name, number_of_players);
            success = matches_monitor.create_match(match_name, number_of_players, duck_info,
                                                   &client_queue);

            // (4)
            protocol.send_confirmation(success);
            break;
        }
        case CMD_JOIN: {
            // (2)
            std::list<std::string> available_matches = matches_monitor.get_available_match_names();

            protocol.send_match_list(available_matches);
            std::cout << "Match list sent\n";
            // (3)
            this->protocol.recv_match_name(match_name);

            // (4)
            success = matches_monitor.join_match(match_name, duck_info, &client_queue);
            protocol.send_confirmation(success);
            break;
        }
    }
    if (success) {
        // El atributo color podria no estar, y que se le asocie
        // un color del lado del cliente en base al id (que siempre
        // va a ser unico).
        duck_info.color = static_cast<char>(duck_info.id);

        protocol.send_duck_unique_attributes(duck_info);

        identity.name = player_name;
        identity.joined_match_name = match_name;
        identity.id = duck_info.id;
    }
}
