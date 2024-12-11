#include <cassert>
#include <chrono>
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
#include "../server/server_protocol.cpp"
#include "../common/types/constants.h"


void test_create_command() {
    Socket socketServer("8080");
    Socket socketCliente("localhost", "8080");
    Socket socket_accepted = socketServer.accept();
    ServerProtocol serverProtocol(socket_accepted);
    LobbyProtocol lobbyProtocol(socketCliente);
    lobbyProtocol.sendCreateCommand("Pedro");
    char lobby_action;
    serverProtocol.recv_action(lobby_action);
    assert(lobby_action == CMD_CREATE);
    std::cout << "Test 1 passed" << std::endl;
}

void test_join_command() {
    Socket socketServer("8080");
    Socket socketCliente("localhost", "8080");
    Socket socket_accepted = socketServer.accept();
    ServerProtocol serverProtocol(socket_accepted);
    LobbyProtocol lobbyProtocol(socketCliente);
    lobbyProtocol.sendJoinCommand("Pedro");
    char lobby_action;
    serverProtocol.recv_action(lobby_action);
    assert(lobby_action == CMD_JOIN);
    std::cout << "Test 2 passed" << std::endl;
}

void test_receive_maps() {
    Socket socketServer("8080");
    Socket socketCliente("localhost", "8080");
    Socket socket_accepted = socketServer.accept();
    ServerProtocol serverProtocol(socket_accepted);
    LobbyProtocol lobbyProtocol(socketCliente);

    std::list<std::string> maps = {"mapa1", "mapa2", "mapa3"};
    serverProtocol.send_game_map_list(maps);
    std::vector<std::string> received_maps = lobbyProtocol.receiveMapList();

    assert(received_maps.size() == 3);
    assert(received_maps[0] == "mapa1");
    assert(received_maps[1] == "mapa2");
    assert(received_maps[2] == "mapa3");

    std::cout << "Test 3 passed" << std::endl;
}

int main() {
    test_create_command();
    test_join_command();
    test_receive_maps();
    return 0;
}