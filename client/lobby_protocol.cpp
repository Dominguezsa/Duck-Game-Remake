#include "lobby_protocol.h"

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
    uint8_t cmd = CMD_CREATE;
    std::vector<uint8_t> buffer;
    buffer.push_back(cmd);
    buffer.push_back(numPlayers);
    buffer.insert(buffer.end(), matchName.begin(), matchName.end());
    buffer.push_back('\0');
    buffer.insert(buffer.end(), mapName.begin(), mapName.end());
    buffer.push_back('\0');

    send_data(buffer.data(), buffer.size());

    // Receive confirmation byte
    uint8_t confirmation;
    recv_uint_8(confirmation);
    return confirmation;
    /*
    if (confirmation == SUCCESS) {
        auto initialState = std::make_shared<MatchInitialState>();
        initialState->duck_identities = std::make_shared<std::vector<DuckIdentity>>();
        initialState->duck_identities->push_back(receiveDuckIdentity());
        return initialState;
    }
    return nullptr;*/
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
    /*
    if (confirmation == SUCCESS) {
        auto initialState = std::make_shared<MatchInitialState>();
        initialState->duck_identities = std::make_shared<std::vector<DuckIdentity>>();
        initialState->duck_identities->push_back(receiveDuckIdentity());
        return initialState;
    }
    return nullptr;*/
}
/*
DuckIdentity LobbyProtocol::receiveDuckIdentity() {
    DuckIdentity identity;
    recv_string(identity.name);
    recv_uint_8(identity.id);
    uint8_t color;
    recv_uint_8(color);
    identity.color = static_cast<char>(color);
    float pos_x, pos_y;
    recv_float(pos_x);
    recv_float(pos_y);
    identity.initial_pos_x = pos_x;
    identity.initial_pos_y = pos_y;
    return identity;
}*/
