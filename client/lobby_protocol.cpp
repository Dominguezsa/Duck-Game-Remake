#include "lobby_protocol.h"

#include <iostream>
#include <stdexcept>

LobbyProtocol::LobbyProtocol(Socket& socket): Protocol(socket) {}

void LobbyProtocol::read_msg(void* msg) { (void)msg; }

void LobbyProtocol::send_msg(void* msg) { (void)msg; }

void LobbyProtocol::sendCreateCommand(const std::string& playerName) {
    uint8_t cmd = CMD_CREATE;
    send_data(&cmd, sizeof(cmd));
    send_string(playerName);
}

void LobbyProtocol::sendJoinCommand(const std::string& playerName) {
    uint8_t cmd = CMD_JOIN;
    send_data(&cmd, sizeof(cmd));
    send_string(playerName);
}

std::vector<std::string> LobbyProtocol::receiveMapList() {
    std::vector<std::string> maps;
    uint16_t count;
    recv_uint_16(count);
    for (uint16_t i = 0; i < count; i++) {
        std::string mapName;
        recv_string(mapName);
        maps.push_back(mapName);
    }
    return maps;
}


int LobbyProtocol::sendMatchCreation(uint8_t numPlayers, const std::string& matchName,
                                     const std::string& mapName) {

    send_data(&CMD_CREATE, sizeof(CMD_CREATE));
    send_data(&numPlayers, sizeof(numPlayers));
    uint16_t matchNameSize = matchName.size();
    matchNameSize = htons(matchNameSize);
    send_data(&matchNameSize, sizeof(matchNameSize));
    send_data(matchName.c_str(), matchName.size());
    uint16_t mapNameSize = mapName.size();
    mapNameSize = htons(mapNameSize);
    send_data(&mapNameSize, sizeof(mapNameSize));
    send_data(mapName.c_str(), mapName.size());

    // Receive confirmation byte
    uint8_t confirmation;
    recv_uint_8(confirmation);
    return confirmation;
}

std::vector<std::string> LobbyProtocol::receiveMatchList() {
    std::vector<std::string> matches;
    uint16_t count;
    recv_uint_16(count);
    for (uint16_t i = 0; i < count; i++) {
        std::string matchName;
        recv_string(matchName);
        matches.push_back(matchName);
    }
    return matches;
}

int LobbyProtocol::sendMatchSelection(const std::string& matchName) {
    send_string(matchName);

    // Receive confirmation byte
    uint8_t confirmation;
    recv_uint_8(confirmation);
    return confirmation;
}
