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

    // Create a new game
    // std::shared_ptr<MatchInitialState> createGame(const std::string& playerName, uint8_t
    // numPlayers,
    //                                              const std::string& matchName,
    //                                             const std::string& mapName);

    // Join an existing game
    // std::shared_ptr<MatchInitialState> joinGame(const std::string& playerName,
    //                                           const std::string& matchName);

    // Get available maps (for game creation)
    // std::vector<std::string> getAvailableMaps(const std::string& playerName);

    // Get available matches (for joining)
    // std::vector<std::string> getAvailableMatches(const std::string& playerName);

    // Transfer socket ownership (when transitioning to game)
    // Socket transferSocket();

    void run();

    // Clean up resources
    void quit();

    void handle_create_party();

    void handle_join_party();

    ~Lobby();
};

#endif  // LOBBY_H
