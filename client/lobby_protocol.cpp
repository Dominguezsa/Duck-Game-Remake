#include "lobby_protocol.h"
#include <memory>
#include <cstdint>

#define MAX_LEN 256 
#include <netinet/in.h>
#include <iostream>
#include "../common/common_liberror.h"

LobbyProtocol::LobbyProtocol(Socket& a_skt) : was_closed(false), skt(a_skt) {}

uint16_t LobbyProtocol::receive_header() {
    uint16_t header = 0;
    this->skt.recvall(&header, sizeof(uint16_t), &was_closed);
    header = ntohs(header);
    this->skt_was_closed();
    return header;
}

GameMatchInfo LobbyProtocol::receive_game() {
    try {
        GameMatchInfo single_game_info;
        skt.recvall(&single_game_info, sizeof(single_game_info), &was_closed);
        this->skt_was_closed();
        single_game_info.str_len = ntohs(single_game_info.str_len);
        single_game_info.player_count = ntohs(single_game_info.player_count);
        return single_game_info;
    } catch (const LibError& skt_err) {
        std::string errorMessage = 
            "Some error occurred while trying to receive a message from the server.";
        throw std::runtime_error(errorMessage);
    }
}

uint8_t LobbyProtocol::send_selected_game(const std::vector<char>& gamenameToSend,
                                         char user_character,
                                         const std::vector<char>& usernameToSend) {
    try {
        uint8_t game_option = NEW_GAME;
        skt.sendall(&game_option, sizeof(uint8_t), &was_closed);
        PlayerInfo player_info;

        player_info.character_code = user_character;
        player_info.str_len = (uint16_t)usernameToSend.size();

        // Copy username with null padding
        size_t i = 0;
        for (i = 0; i < usernameToSend.size(); i++) {
            player_info.player_name[i] = usernameToSend.at(i);
        }
        player_info.player_name[i] = '\0';
        i++;
        for (size_t j = i; j < MAX_LEN; j++) {
            player_info.player_name[j] = 0;
        }

        // Copy game name with null padding
        i = 0;
        for (i = 0; i < gamenameToSend.size(); i++) {
            player_info.game_name[i] = gamenameToSend.at(i);
        }
        player_info.game_name[i] = '\0';
        i++;
        for (size_t j = i; j < MAX_LEN; j++) {
            player_info.game_name[j] = 0;
        }

        skt.sendall(&player_info, sizeof(player_info), &was_closed);
        this->skt_was_closed();
        return this->receive_player_id();
    } catch (const LibError& skt_err) {
        std::cout << "Some error occurred while trying to send a message to the server."
                  << std::endl;
        return ERROR;
    }
}

void LobbyProtocol::send_refresh() {
    try {
        uint8_t refresh = REFRESH;
        skt.sendall(&refresh, sizeof(uint8_t), &was_closed);
        this->skt_was_closed();
    } catch (const LibError& skt_err) {
        std::cout << "Some error occurred while trying to send a message to the server."
                  << std::endl;
    }
}

uint8_t LobbyProtocol::receive_player_id() {
    try {
        uint8_t player_id = 0;
        skt.recvall(&player_id, sizeof(uint8_t), &was_closed);
        this->skt_was_closed();
        return player_id;
    } catch (const LibError& skt_err) {
        std::cout << "Some error occurred while trying to receive a message from the server."
                  << std::endl;
        return ERROR;
    }
}

void LobbyProtocol::skt_was_closed() {
    if (was_closed) {
        std::string errorMessage = 
            "Some error occurred while trying to receive a message from the server.";
        throw std::runtime_error(errorMessage);
    }
}