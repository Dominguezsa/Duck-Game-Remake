#ifndef LOBBY_H
#define LOBBY_H

#include <memory>
#include <string>
#include <vector>

#include "../common/common_socket.h"

#include "lobby_protocol.h"

class Lobby {
private:
    Socket& skt;
    LobbyProtocol protocol;
    bool skt_ownership;
    bool is_connected;
    std::string playerName;

public:
    explicit Lobby(Socket& s);

    void run();

    void quit();

    void handle_create_party();

    void handle_join_party();

    ~Lobby();
};

#endif  // LOBBY_H
