#include "lobby.h"

#include <iostream>
#include <stdexcept>
#include <utility>

Lobby::Lobby(Socket& s): skt(std::move(s)), protocol(s), skt_ownership(true), is_connected(true) {}

std::shared_ptr<MatchInitialState> Lobby::createGame(const std::string& playerName, uint8_t numPlayers, 
                                                    const std::string& matchName, const std::string& mapName) {
    if (!is_connected) return nullptr;
    
    try {
        protocol.sendCreateCommand(playerName);
        auto maps = protocol.receiveMapList();
        if (maps.empty()) return nullptr;
        
        return protocol.sendMatchCreation(numPlayers, matchName, mapName);
    } catch (const std::exception& e) {
        is_connected = false;
        return nullptr;
    }
}
std::shared_ptr<MatchInitialState> Lobby::joinGame(const std::string& playerName, const std::string& matchName) {
    if (!is_connected) return nullptr;
    
    try {
        protocol.sendJoinCommand(playerName);
        auto matches = protocol.receiveMatchList();
        if (matches.empty()) return nullptr;
        
        return protocol.sendMatchSelection(matchName);
    } catch (const std::exception& e) {
        is_connected = false;
        return nullptr;
    }
}

std::vector<std::string> Lobby::getAvailableMaps(const std::string& playerName) {
    if (!is_connected)
        return std::vector<std::string>();

    try {
        protocol.sendCreateCommand(playerName);
        return protocol.receiveMapList();
    } catch (const std::exception& e) {
        is_connected = false;
        return std::vector<std::string>();
    }
}

std::vector<std::string> Lobby::getAvailableMatches(const std::string& playerName) {
    if (!is_connected)
        return std::vector<std::string>();

    try {
        protocol.sendJoinCommand(playerName);
        return protocol.receiveMatchList();
    } catch (const std::exception& e) {
        is_connected = false;
        return std::vector<std::string>();
    }
}

void Lobby::run() {
    if (playerName.empty()) {
        std::cout << "What is your name?\n";
        std::cin >> playerName;
    }
    bool invalid_option = true;
    char option = ' ';
    while (invalid_option) {
        std::cout << "Do you wanna create ('c') or join a match ('j')?\n";
        std::cin >> option;
        if (option != 'c' && option != 'j') {
            std::cout << "Invalid option\n";
            continue;
        } else {
            invalid_option = false;
        }
    }
    if (option == 'c') {
        handle_create_party();
    } else if (option == 'j') {
        handle_join_party();
    }
}

void Lobby::handle_create_party() {
    // enviar C LEN_NAME NAME
    // recibir mapas
    // mostrar mapas
    uint8_t numPlayers;
    std::string matchName;
    std::cout << "Enter the match name\n";
    std::cin >> matchName;
    std::cout << "Enter the number of players\n";
    std::cin >> numPlayers;
    // createGame(numPlayers, matchName))
    // imprimir si se creo la partida correctamente o no
}

void Lobby::handle_join_party() {
    // enviar J LEN_NAME NAME
    // recibir partidas
    // mostrar partidas
    std::string matchName;
    std::cout << "Enter the party you wanna join (select the numbre f.e: 1,2,3,...)\n";
    std::cin >> matchName;
    // joinGame(matchName)
    // imprimir si se unio a la partida correctamente o no
}


void Lobby::quit() {
    if (skt_ownership) {
        protocol.end_communication();
    }
    is_connected = false;
}

Lobby::~Lobby() { quit(); }
