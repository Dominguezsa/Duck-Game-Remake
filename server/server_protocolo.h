#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <cstdint>
#include <list>
#include <vector>

#include "../common/common_socket.h"
#include "../common/common_protocol.h"
#include "server_gameloop_message.h"
//#include "../common/duck.h"
#include "../common/types/duck_state.h"

class ServerProtocol : public Protocol {
public:
    //Socket& socket;
    //uint8_t jugador_id; // Que el sender lo asigne.

    explicit ServerProtocol(Socket &skt):Protocol(skt) {}

    void recv_msg(GameloopMessage& msg);

    void send_duck_states(std::vector<DuckState>& states);

    void apagarSocket();
};

#endif  // SERVER_PROTOCOLO_H
