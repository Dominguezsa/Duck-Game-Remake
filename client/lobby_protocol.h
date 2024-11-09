#ifndef LOBBY_PROTOCOL_H
#define LOBBY_PROTOCOL_H
#include "../common/common_protocol.h"
#include "../common/common_socket.h"
#include <cstdint>
#include <string>
#include <vector>

class LobbyProtocol : public Protocol {
public:
    explicit LobbyProtocol(Socket& socket);

    // Added override keyword
    void read_msg(void* msg) override;
    void send_msg(void* msg) override;

    
    void sendCreateCommand(const std::string& playerName);
    std::vector<std::string> receiveMapList();
    bool sendMatchCreation(uint8_t numPlayers, const std::string& matchName);
    void sendJoinCommand(const std::string& playerName);
    std::vector<std::string> receiveMatchList();
    bool sendMatchSelection(const std::string& matchName);

private:
    bool receiveConfirmation();

    static constexpr char CMD_CREATE = 'C';
    static constexpr char CMD_JOIN = 'J';
    static constexpr uint8_t SUCCESS = 0x01;
    static constexpr uint8_t FAILURE = 0x00;
};
#endif