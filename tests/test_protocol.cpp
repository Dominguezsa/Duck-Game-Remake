#include <iostream>
#include <sstream>

#include "../client/lobby_protocol.cpp"
#include "../common/common_liberror.cpp"
#include "../common/common_protocol.cpp"
#include "../common/common_queue.h"
#include "../common/common_resolver.cpp"
#include "../common/common_resolvererror.cpp"
#include "../common/common_socket.cpp"
#include "../common/common_thread.h"
#include "../common/types/constants.h"
#include "../server/server_protocol.cpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#define port "8080"
#define ip "localhost"

using ::testing::AllOf;
using ::testing::HasSubstr;
using ::testing::ThrowsMessage;

class TestProtocol: public ::testing::Test {
protected:
    Socket socketServer;
    Socket socketCliente;
    Socket socket_accepted;
    ServerProtocol serverProtocol;
    LobbyProtocol lobbyProtocol;
    TestProtocol():
            socketServer(port),
            socketCliente(ip, port),
            socket_accepted(socketServer.accept()),
            serverProtocol(socket_accepted),
            lobbyProtocol(socketCliente) {}
};

namespace {

TEST_F(TestProtocol, CreateCommandCheck) {
    lobbyProtocol.sendCreateCommand("Pedro");
    char lobby_action;
    serverProtocol.recv_action(lobby_action);
    EXPECT_EQ(lobby_action, CMD_CREATE);
}


TEST_F(TestProtocol, JoinCommandCheck) {
    lobbyProtocol.sendJoinCommand("Pedro");
    char lobby_action;
    std::string player_name;
    serverProtocol.recv_action(lobby_action);
    serverProtocol.recv_player_name(player_name);
    EXPECT_EQ(lobby_action, CMD_JOIN);
    EXPECT_EQ(player_name, "Pedro");
}


TEST_F(TestProtocol, ReceiveMaps) {
    std::list<std::string> maps = {"mapa1", "mapa2", "mapa3"};
    serverProtocol.send_game_map_list(maps);

    std::vector<std::string> received_maps = lobbyProtocol.receiveMapList();

    ASSERT_EQ(received_maps.size(), 3);
    EXPECT_EQ(received_maps[0], "mapa1");
    EXPECT_EQ(received_maps[1], "mapa2");
    EXPECT_EQ(received_maps[2], "mapa3");
}

TEST_F(TestProtocol, SendMatchCreation) {
    lobbyProtocol.sendMatchCreation(2, "match_name_pedro", "map_name1");

    uint8_t number_of_players;
    std::string map_name;
    std::string match_name;
    serverProtocol.recv_match_info(map_name, match_name, number_of_players);

    EXPECT_EQ(number_of_players, 2);
    EXPECT_EQ(map_name, "map_name1");
    EXPECT_EQ(match_name, "match_name_pedro");
}


TEST_F(TestProtocol, ReceiveConfirmation) {
    serverProtocol.send_confirmation(true);
    uint8_t confirmation = lobbyProtocol.receiveConfirmation();
    EXPECT_EQ(confirmation, SUCCESS);

    serverProtocol.send_confirmation(false);
    confirmation = lobbyProtocol.receiveConfirmation();
    EXPECT_EQ(confirmation, FAILURE);
}

}  // namespace
