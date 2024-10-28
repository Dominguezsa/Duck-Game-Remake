#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <cstdint>
#include <list>
#include <vector>
#include <iostream>

#include "../common/common_socket.h"
#include "../common/common_protocol.h"
#include "server_gameloop_message.h"
#include "../common/types/duck_state.h"

class ServerProtocol : public Protocol {
public:
    explicit ServerProtocol(Socket &skt) : Protocol(skt) {}

    void recv_msg(uint8_t &command);

    void send_duck_states(std::vector<DuckState>& states);

    // No se usan! Quitar despues de common/common_protocol.h
    void read_msg(void *msg) override;

    void send_msg(void *msg) override;
};

#endif  // SERVER_PROTOCOLO_H
