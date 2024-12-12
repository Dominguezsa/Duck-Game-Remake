#include <iostream>
#include <sstream>

#include "../client/lobby_protocol.cpp"
#include "../client/protocol.cpp"
#include "../common/common_protocol.cpp"
#include "../common/common_queue.h"
#include "../common/common_resolver.cpp"
#include "../common/common_resolvererror.cpp"
#include "../common/common_socket.cpp"
#include "../common/common_thread.h"
#include "../common/snapshot.h"
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
    ClientProtocol clientProtocol;
    TestProtocol():
            socketServer(port),
            socketCliente(ip, port),
            socket_accepted(socketServer.accept()),
            serverProtocol(socket_accepted),
            lobbyProtocol(socketCliente),
            clientProtocol(socketCliente) {}
};

namespace {

/* MESSAGES TESTED FOR SERVER:

    * - recv_lobby_action (TestCreateCommandCheck, TestJoinCommandCheck)
    * - recv_string (TestCreateCommandCheck, TestJoinCommandCheck)
    * - send_game_map_list (TestReceiveMaps)
    * - recv_match_info (TestMatchCreation)
    * - send_confirmation (TestConfirmation)
    * - send_match_list (TestMatchList)
    * - recv_duck_action (TestDuckAction)
    * - send_snapshot (TestSnapshot)
*/

/*MESSAGES TESTED FOR CLIENT:

    * - sendCreateCommand (TestCreateCommandCheck)
    * - sendJoinCommand (TestJoinCommandCheck)
    * - receiveMapList (TestReceiveMaps)
    * - sendMatchCreation (TestMatchCreation)
    * - semdMatchSelection (TestMatchSelection)
    * - receiveConfirmation (TestConfirmation)
    * - receiveMatchList (TestMatchList)
    * - send_msg (TestDuckAction)
    * - read_msg (TestSnapshot)
*/

TEST_F(TestProtocol, TestCreateCommandCheck) {
    lobbyProtocol.sendCreateCommand("Pedro");
    char lobby_action;
    std::string player_name;
    serverProtocol.recv_lobby_action(lobby_action);
    serverProtocol.recv_string(player_name);
    EXPECT_EQ(lobby_action, CMD_CREATE);
    EXPECT_EQ(player_name, "Pedro");
}

// cpp-check-suppress syntaxError
TEST_F(TestProtocol, TestJoinCommandCheck) {
    lobbyProtocol.sendJoinCommand("Pedro");
    char lobby_action;
    std::string player_name;
    serverProtocol.recv_lobby_action(lobby_action);
    serverProtocol.recv_string(player_name);
    EXPECT_EQ(lobby_action, CMD_JOIN);
    EXPECT_EQ(player_name, "Pedro");
}


TEST_F(TestProtocol, TestReceiveMaps) {
    std::list<std::string> maps = {"mapa1", "mapa2", "mapa3"};
    serverProtocol.send_game_map_list(maps);

    std::vector<std::string> received_maps = lobbyProtocol.receiveStringVector();

    ASSERT_EQ(received_maps.size(), 3);
    EXPECT_EQ(received_maps[0], "mapa1");
    EXPECT_EQ(received_maps[1], "mapa2");
    EXPECT_EQ(received_maps[2], "mapa3");
}

TEST_F(TestProtocol, TestMatchCreation) {
    lobbyProtocol.sendMatchCreation(2, "match_name_pedro", "map_name1");

    uint8_t number_of_players;
    std::string map_name;
    std::string match_name;
    serverProtocol.recv_match_info(map_name, match_name, number_of_players);

    EXPECT_EQ(number_of_players, 2);
    EXPECT_EQ(map_name, "map_name1");
    EXPECT_EQ(match_name, "match_name_pedro");
}

TEST_F(TestProtocol, TestMatchSelection) {
    lobbyProtocol.sendMatchSelection("match_name_pedro");
    std::string match_name;
    serverProtocol.recv_string(match_name);

    EXPECT_EQ(match_name, "match_name_pedro");
}


TEST_F(TestProtocol, TestConfirmation) {
    serverProtocol.send_confirmation(true);
    uint8_t confirmation = lobbyProtocol.receiveConfirmation();
    EXPECT_EQ(confirmation, SUCCESS);

    serverProtocol.send_confirmation(false);
    confirmation = lobbyProtocol.receiveConfirmation();
    EXPECT_EQ(confirmation, FAILURE);
}

TEST_F(TestProtocol, TestMatchList) {
    serverProtocol.send_match_list({"match1", "match2", "match3"});
    std::vector<std::string> matches = lobbyProtocol.receiveStringVector();

    ASSERT_EQ(matches.size(), 3);
    EXPECT_EQ(matches[0], "match1");
    EXPECT_EQ(matches[1], "match2");
    EXPECT_EQ(matches[2], "match3");
}

TEST_F(TestProtocol, TestDuckAction) {
    clientProtocol.send_msg(static_cast<void*>(new char('c')));
    uint8_t action;
    serverProtocol.recv_duck_action(action);
    EXPECT_EQ(action, 'c');
}

TEST_F(TestProtocol, TestSnapshot) {
    auto snapshot = std::make_shared<Snapshot>();
    snapshot->ducks.push_back(DuckState());
    snapshot->weapons.push_back(Weapon());
    snapshot->bullets.push_back(Bullet());

    serverProtocol.send_snapshot(snapshot);
    Snapshot received_snapshot;
    clientProtocol.read_msg(&received_snapshot);

    EXPECT_EQ(received_snapshot.ducks.size(), 1);
    EXPECT_EQ(received_snapshot.weapons.size(), 1);
    EXPECT_EQ(received_snapshot.bullets.size(), 1);
}

TEST_F(TestProtocol, TestReceivePlatforms) {
    auto snapshot = std::make_shared<Snapshot>();
    snapshot->addPlatform(Platform());
    snapshot->addPlatform(Platform());

    serverProtocol.send_snapshot(snapshot);
    Snapshot received_snapshot;
    clientProtocol.recv_platforms(received_snapshot.platforms);

    EXPECT_EQ(received_snapshot.platforms.size(), 2);
}


}  // namespace
