#include "server_class.h"

#include <utility>
#include <iostream>
#include <string>


Server::Server(std::string port): port(std::move(port)) {}

void Server::execute() {
    AcceptorThread server(port);
    server.start();
    server.stop();
    server.join();
}


void Server::waitQuitInput() {
    std::string entradaDelServer;
    while (getline(std::cin, entradaDelServer)) {
        if (entradaDelServer == "q") {
            break;
        }
    }
}