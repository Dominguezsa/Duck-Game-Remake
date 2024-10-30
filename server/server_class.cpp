#include "server_class.h"

#include <iostream>
#include <string>
#include <utility>


Server::Server(std::string port): port(std::move(port)) {}

void Server::execute() {

    AcceptorThread acceptorThread(port);

    acceptorThread.start();
    waitQuitInput();
    acceptorThread.stop();
    acceptorThread.join();
}


void Server::waitQuitInput() {
    std::string entradaDelServer;
    while (getline(std::cin, entradaDelServer)) {
        if (entradaDelServer == "q") {
            break;
        }
    }
}
