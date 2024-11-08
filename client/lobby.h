#ifndef LOBBY_H
#define LOBBY_H

#include <string>
#include <vector>

#include "../common/common_socket.h"

#include "lobby_protocol.h"

class Lobby {
private:
    Socket skt;
    LobbyProtocol protocol;
    uint8_t player_id;
    bool skt_ownership;
    bool game_started;

public:
    // Constructor that initializes the socket connection with server
    explicit Lobby(Socket s);

    /*
     * Returns all available games from the server
     */
    std::vector<GameMatchInfo> get_games();

    /*
     * Sends refresh request to server and returns updated game list
     */
    std::vector<GameMatchInfo> refresh_games();

    /*
     * Sends selected game info to server:
     * - gamename: name of the selected/created game
     * - user_character: selected duck color/character
     * - username: player's username
     */
    void send_selected_game(const std::string& gamename, char user_character,
                            const std::string& username);

    /*
     * Returns this player's assigned ID
     */
    uint8_t get_player_id();

    /*
     * Waits for game host to start the game
     * Returns initial game snapshot
     */
    DuckState wait_game_start();

    /*
     * Transfers socket ownership (using move semantics)
     */
    Socket transfer_socket();

    /*
     * Closes the connection with server
     */
    void quit_game();

    /*
     * Returns whether the game has started
     */
    bool did_game_start() const;

    /*
     * Destructor - closes socket if ownership hasn't been transferred
     */
    ~Lobby();

    void run();
};

#endif  // LOBBY_H
