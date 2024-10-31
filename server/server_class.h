#ifndef SERVER_CLASS_H
#define SERVER_CLASS_H

#include <string>
#include <tuple>
#include <vector>

#include "../common/common_socket.h"

#include "server_acceptor.h"
#include "server_match.h"

class Server {
private:
    std::string port;

public:
    explicit Server(std::string port);

    void execute();

    static void waitQuitInput();
};

#endif  // SERVER_CLASS_H
