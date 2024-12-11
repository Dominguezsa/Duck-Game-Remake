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

int main() {
    test_create_command();
    return 0;
}