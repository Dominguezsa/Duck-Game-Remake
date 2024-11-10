#include "lobby_protocol.h"

#include <stdexcept>

LobbyProtocol::LobbyProtocol(Socket& socket): Protocol(socket) {}

void LobbyProtocol::read_msg(void* msg) {
    (void)msg;  // Silence unused parameter warning
    // Implementation here if needed
}

void LobbyProtocol::send_msg(void* msg) {
    (void)msg;  // Silence unused parameter warning
    // Implementation here if needed
}

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
    recv_uint_16(count);  // Using inherited method
    for (uint16_t i = 0; i < count; i++) {
        std::string mapName;
        recv_string(mapName);  // Using inherited method
        maps.push_back(mapName);
    }
    return maps;
}

bool LobbyProtocol::sendMatchCreation(uint8_t numPlayers, const std::string& matchName) {
    uint8_t cmd = CMD_CREATE;
    send_data(&cmd, sizeof(cmd));
    send_data(&numPlayers, sizeof(numPlayers));
    send_string(matchName);
    return receiveConfirmation();
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

bool LobbyProtocol::sendMatchSelection(const std::string& matchName) {
    send_string(matchName);
    return receiveConfirmation();
}

bool LobbyProtocol::receiveConfirmation() {
    uint8_t confirmation;
    recv_uint_8(confirmation);
    return confirmation == SUCCESS;
}
