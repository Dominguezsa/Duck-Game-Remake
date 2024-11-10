#include "lobby.h"

#include <iostream>
#include <stdexcept>
#include <utility>

Lobby::Lobby(Socket& s): skt(std::move(s)), protocol(s), skt_ownership(true), is_connected(true) {}

bool Lobby::createGame(const std::string& playerName, uint8_t numPlayers,
                       const std::string& matchName) {
    if (!is_connected)
        return false;

    try {
        // Send create command and player name
        protocol.sendCreateCommand(playerName);

        // Get available maps
        auto maps = protocol.receiveMapList();
        if (maps.empty())
            return false;

        // Create match
        return protocol.sendMatchCreation(numPlayers, matchName);
    } catch (const std::exception& e) {
        is_connected = false;
        return false;
    }
}

bool Lobby::joinGame(const std::string& playerName, const std::string& matchName) {
    if (!is_connected)
        return false;

    try {
        // Send join command and player name
        protocol.sendJoinCommand(playerName);

        // Get available matches
        auto matches = protocol.receiveMatchList();
        if (matches.empty())
            return false;

        // Select match
        return protocol.sendMatchSelection(matchName);
    } catch (const std::exception& e) {
        is_connected = false;
        return false;
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
    while (invalid_option) {
        std::cout << "Do you wanna create ('c') or join a match ('j')?\n";
        char option;
        std::cin >> option;
        if (option != 'c' && option != 'j') {
            std::cout << "Invalid option\n";
            continue;
        } else {
            invalid_option = false;
        }
    }
    std::cout << "Enter the party id\n";
    std::string id;
    std::cin >> id;
}


void Lobby::quit() {
    if (skt_ownership) {
        protocol.end_communication();
    }
    is_connected = false;
}

Lobby::~Lobby() { quit(); }
