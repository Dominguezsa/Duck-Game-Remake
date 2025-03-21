#include "server_client_session.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <utility>

#include <yaml-cpp/yaml.h>
ClientSession::ClientSession(Socket _skt, MatchesMonitor& monitor):
        identity(),
        skt(std::move(_skt)),
        protocol(this->skt),
        matches_monitor(monitor),
        client_queue(CLIENT_QUEUE_SIZE) {}

uint8_t ClientSession::get_id() const { return identity.id; }

void ClientSession::end_communication() {
    try {
        // Closing the communication channel.
        protocol.end_communication();
    } catch (...) {
        std::string err_msg =
                "Something went wrong closing communication with " + identity.name + ".\n";
        syslog(LOG_ERR, "%s", err_msg.c_str());
    }
}

void ClientSession::stop() {
    try {
        this->_is_alive = false;
        end_communication();
        this->client_queue.close();
    } catch (...) {
        std::string err_msg =
                "Something went wrong stopping communication with " + identity.name + ".\n";
        syslog(LOG_ERR, "%s", err_msg.c_str());
    }
}

Queue<GameloopMessage>* ClientSession::get_match_queue() {
    return matches_monitor.get_match_queue(identity.joined_match_name);
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
            ReceiverThread receiver(*get_match_queue(), protocol, identity.id);
            sender.start();
            receiver.start();
            while (receiver.is_alive() && sender.is_alive()) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            this->matches_monitor.disconnect_player(identity.joined_match_name, identity.id);
            sender.stop();
            sender.join();
            receiver.stop();
            receiver.join();
        }
    } catch (const SocketWasCLosedException& e) {
        this->_is_alive = false;
        end_communication();
    } catch (...) {
        this->_is_alive = false;
        end_communication();
    }
}

void ClientSession::run_lobby_loop() {
    bool success = false;
    try {
        while (true) {
            char lobby_action;
            this->protocol.recv_lobby_action(lobby_action);
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
            this->matches_monitor.disconnect_player(identity.joined_match_name, identity.id);
            end_communication();
        }
        this->_is_alive = false;
    }
}

void ClientSession::exec_lobby_action(char action, bool& success) {
    std::string player_name, match_name;
    this->protocol.recv_string(player_name);
    DuckIdentity duck_info;
    identity.name = player_name;
    duck_info.name = player_name;

    switch (action) {
        case CMD_CREATE: {
            std::list<std::string> map_list;
            get_available_maps(map_list);
            protocol.send_game_map_list(map_list);

            uint8_t number_of_players;
            std::string map_name;
            this->protocol.recv_match_info(map_name, match_name, number_of_players);
            success = matches_monitor.create_match(match_name, number_of_players, duck_info,
                                                   &client_queue, matches[map_name]);

            protocol.send_confirmation(success);
            break;
        }
        case CMD_JOIN: {
            std::list<std::string> available_matches = matches_monitor.get_available_match_names();

            protocol.send_match_list(available_matches);
            this->protocol.recv_string(match_name);

            success = matches_monitor.join_match(match_name, duck_info, &client_queue);
            protocol.send_confirmation(success);
            break;
        }
    }
    if (success) {
        identity.name = player_name;
        identity.joined_match_name = match_name;
        identity.id = duck_info.id;
    }
}

void ClientSession::get_available_maps(std::list<std::string>& map_list) {
    // Ruta de la carpeta "maps"
    namespace fs = std::filesystem;
    fs::path maps_path = RESOURCE_PATH "/maps/";

    // Verificar si la carpeta existe
    if (!fs::exists(maps_path) || !fs::is_directory(maps_path)) {
        std::cerr << "Error: La carpeta 'maps' no existe o no es un directorio." << std::endl;
        return;
    }

    // Iterar sobre los archivos en la carpeta "maps"
    for (const auto& entry: fs::directory_iterator(maps_path)) {
        // Solo procesar archivos con extensión .yaml
        if (entry.is_regular_file() && entry.path().extension() == ".yaml") {
            try {
                // Abrir y parsear el archivo YAML
                YAML::Node map_node = YAML::LoadFile(entry.path().string());

                // Extraer el nombre del mapa
                if (map_node["name"]) {
                    std::string map_name = map_node["name"].as<std::string>();
                    map_list.push_back(map_name);
                } else {
                    std::cerr << "Advertencia: El archivo " << entry.path().filename()
                              << " no contiene un campo 'name'." << std::endl;
                    continue;
                }

                // Extraer las tiles
                std::vector<Platform> platforms;
                std::vector<Respawn> respawns;
                std::vector<weapons_in_map> weapons;

                if (map_node["tiles"]) {
                    const auto& tiles = map_node["tiles"];
                    for (size_t row = 0; row < tiles.size(); ++row) {
                        for (size_t col = 0; col < tiles[row].size(); ++col) {
                            int tile_value = tiles[row][col].as<int>();

                            float x = col * 50.0f;
                            float y = row * 50.0f;

                            if (tile_value >= 1 && tile_value <= 12) {
                                // Crear plataforma
                                platforms.emplace_back(Platform{x, y, 50.0f, 50.0f,
                                                                static_cast<uint8_t>(tile_value)});
                            } else if (tile_value == 13) {
                                // Crear punto de respawn
                                respawns.emplace_back(Respawn{x, y});
                            } else if (tile_value > 15 && tile_value <= 25) {
                                // Crear arma
                                weapons.emplace_back(
                                        weapons_in_map{x, y, static_cast<uint8_t>(tile_value)});
                            }
                        }
                    }
                }

                // Crear MapInfo con las plataformas, respawns y armas
                MapInfo map_info(platforms, respawns, weapons);
                matches[map_list.back()] = map_info;

            } catch (const std::exception& e) {
                std::cerr << "Error al procesar el archivo " << entry.path().filename() << ": "
                          << e.what() << std::endl;
            }
        }
    }
}
