#include "lobby.h"

#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <utility>

Lobby::Lobby(Socket& s): skt(s), protocol(s), skt_ownership(true), is_connected(true) {}


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

    skt_ownership = false;
}

void Lobby::handle_create_party() {
    protocol.sendCreateCommand(playerName);
    auto maps = protocol.receiveStringVector();
    std::cout << "These are the available maps:\n";
    for (size_t i = 0; i < maps.size(); i++) {
        std::cout << i + 1 << ". " << maps[i] << '\n';
    }
    std::cout << "Enter the map you wanna play (select the number f.e: 1,2,3,...)\n";
    int mapIndex = 0;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string serverInput = "";
    while (getline(std::cin, serverInput)) {
        try {
            mapIndex = std::stoi(serverInput);
            if (mapIndex < 1 || mapIndex > (int)maps.size()) {
                std::cout << "Invalid map\n";
            } else {
                break;
            }
        } catch (const std::invalid_argument& e) {
            std::cout << "Invalid input\n";
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
    protocol.sendMatchCreation(numPlayersUint8, matchName, maps[mapIndex]);
    int confirmation = protocol.receiveConfirmation();
    if (confirmation == SUCCESS) {
        std::cout << "Match created successfully\n";
    } else {
        std::cout << "Match creation failed\n";
        run();
    }
}

void Lobby::handle_join_party() {
    protocol.sendJoinCommand(playerName);
    std::vector<std::string> matches = protocol.receiveStringVector();
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
    protocol.sendMatchSelection(matchName);
    int confirmation = protocol.receiveConfirmation();
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
