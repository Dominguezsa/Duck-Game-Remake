#ifndef LOBBY_PROTOCOL_H
#define LOBBY_PROTOCOL_H

#include <cstdint>
#include <string>
#include <vector>

#include "../common/common_protocol.h"
#include "../common/common_socket.h"
#include "../common/types/duck_state.h"

// Protocol commands
#define REFRESH 1
#define NEW_GAME 2
#define JOIN_GAME 3
#define START_GAME 4
#define ERROR -1

// Matching server-side structs
struct GameMatchInfo {
    // cppcheck-suppress unusedStructMember
    uint16_t str_len;
    // cppcheck-suppress unusedStructMember
    uint16_t player_count;
    // cppcheck-suppress unusedStructMember
    char game_name[256];  // MAX_LEN needs to match server
};

struct PlayerInfo {
    // cppcheck-suppress unusedStructMember
    uint16_t str_len;
    // cppcheck-suppress unusedStructMember
    char character_code;
    // cppcheck-suppress unusedStructMember
    char player_name[256];
    // cppcheck-suppress unusedStructMember
    char game_name[256];
};

class LobbyProtocol: public Protocol {

public:
    explicit LobbyProtocol(Socket& skt);

    // Receives number of available games
    uint16_t receive_header();

    void read_msg(void* msg) override;

    void send_msg(void* msg) override;

    // Receives info about a single game
    GameMatchInfo receive_game();

    // Sends refresh request to server
    void send_refresh();

    // Sends selected game info and returns player ID
    uint8_t send_selected_game(const std::vector<char>& gamenameToSend, char user_character,
                               const std::vector<char>& usernameToSend);

    // Receives player ID from server
    uint8_t receive_player_id();

    // Waits for game start signal and returns initial snapshot
    DuckState wait_game_start(bool& game_started);
};

#endif  // LOBBY_PROTOCOL_H
