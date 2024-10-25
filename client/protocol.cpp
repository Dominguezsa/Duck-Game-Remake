#include "protocol.h"

#include <string>
#include <iostream>

ClientProtocol::ClientProtocol(Socket& socket): Protocol(socket) {}

std::vector<Duck> ClientProtocol::receiveMessage() {
    std::vector<Duck> ducks;
    return ducks;
}

void ClientProtocol::read_msg(void *msg) {
    std::cout << msg << std::endl;
}

void ClientProtocol::send_msg(void *msg) {
    int int_msg = *static_cast<int*>(msg);
    std::cout << "Sending message: " << int_msg << std::endl;
}
