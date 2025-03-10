#ifndef LOBBY_PROTOCOL_H
#define LOBBY_PROTOCOL_H
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "../common/common_protocol.h"
#include "../common/common_socket.h"
#include "../common/types/constants.h"
#include "../common/types/match_state.h"

class LobbyProtocol: public Protocol {
public:
    explicit LobbyProtocol(Socket& socket);

    void read_msg(void* msg) override;  // no se usa
    void send_msg(void* msg) override;  // no se usa

    void sendCreateCommand(const std::string& playerName);
    std::vector<std::string> receiveStringVector();
    void sendMatchCreation(uint8_t numPlayers, const std::string& matchName,
                           const std::string& mapName);

    int receiveConfirmation();
    void sendJoinCommand(const std::string& playerName);
    void sendMatchSelection(const std::string& matchName);
};
#endif
