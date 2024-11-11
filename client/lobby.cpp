#include "lobby.h"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

Lobby::Lobby(Socket& s): skt(s), protocol(s), skt_ownership(true), is_connected(true) {}
/*
std::shared_ptr<MatchInitialState> Lobby::createGame(const std::string& playerName,
                                                     uint8_t numPlayers,
                                                     const std::string& matchName,
                                                     const std::string& mapName) {
    if (!is_connected)
        return nullptr;

    try {
        protocol.sendCreateCommand(playerName);
        auto maps = protocol.receiveMapList();
        if (maps.empty())
            return nullptr;

        return protocol.sendMatchCreation(numPlayers, matchName, mapName);
    } catch (const std::exception& e) {
        is_connected = false;
        return nullptr;
    }
}
std::shared_ptr<MatchInitialState> Lobby::joinGame(const std::string& playerName,
                                                   const std::string& matchName) {
    if (!is_connected)
        return nullptr;

    try {
        protocol.sendJoinCommand(playerName);
        auto matches = protocol.receiveMatchList();
        if (matches.empty())
            return nullptr;

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
*/
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
    protocol.sendCreateCommand(playerName);
    auto maps = protocol.receiveMapList();
    for (size_t i = 0; i < maps.size(); i++) {
        std::cout << i + 1 << ". " << maps[i] << '\n';
    }
    int mapIndex = 0;
    while (true) {
        std::cout << "Select the map you wanna play\n";
        std::cin >> mapIndex;
        if (mapIndex < 1 || mapIndex > (int)maps.size()) {
            std::cout << "Invalid map\n";
        } else {
            break;
        }
    }
    mapIndex--;
    unsigned int numPlayers;
    std::string matchName;
    std::cout << "Enter the match name\n";
    std::cin >> matchName;
    while (true) {
        std::cout << "Enter the number of players\n";
        std::cin >> numPlayers;
        if (numPlayers < 2 || numPlayers > 4) {
            std::cout << "Invalid number of players\n";
        } else {
            break;
        }
    }
    uint8_t numPlayersUint8 = static_cast<uint8_t>(numPlayers);
    int confirmation = protocol.sendMatchCreation(numPlayersUint8, matchName, maps[mapIndex]);
    if (confirmation == 1) {
        std::cout << "Match created successfully\n";
    } else {
        std::cout << "Match creation failed\n";
        run();
    }
}

void Lobby::handle_join_party() {
    protocol.sendJoinCommand(playerName);
    std::vector<std::string> matches = protocol.receiveMatchList();
    for (size_t i = 0; i < matches.size(); i++) {
        std::cout << i + 1 << ". " << matches[i] << '\n';
    }
    std::string matchName;
    std::cout << "Enter the party you wanna join (select the numbre f.e: 1,2,3,...)\n";
    while (true) {
        std::cin >> matchName;
        if (std::stoi(matchName) < 1 || std::stoi(matchName) > (int)matches.size()) {
            std::cout << "Invalid party\n";
        } else {
            break;
        }
    }
    matchName = matches[std::stoi(matchName) - 1];
    int confirmation = protocol.sendMatchSelection(matchName);
    if (confirmation == 1) {
        std::cout << "Match joined successfully\n";
    } else {
        std::cout << "Match join failed\n";
        run();
    }
}


void Lobby::quit() {
    if (skt_ownership) {
        protocol.end_communication();
    }
    is_connected = false;
}

Lobby::~Lobby() { quit(); }
