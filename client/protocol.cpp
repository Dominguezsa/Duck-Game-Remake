#include "protocol.h"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

ClientProtocol::ClientProtocol(Socket& socket): Protocol(socket) {}

std::vector<Duck> ClientProtocol::receiveMessage() {
    // Placeholder
    std::vector<Duck> ducks;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return ducks;
}

void ClientProtocol::read_msg(void* msg) { std::cout << msg << std::endl; }

void ClientProtocol::send_msg(void* msg) {
    uint8_t message = *static_cast<uint8_t*>(msg);
    // int int_msg = *static_cast<int*>(msg);
    std::cout << "Sending message: " << std::hex << +message << std::endl;
}
