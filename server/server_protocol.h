#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <cstdint>
#include <iostream>
#include <list>
#include <memory>
#include <vector>

#include "../common/common_protocol.h"
#include "../common/common_socket.h"
#include "../common/types/duck_state.h"
#include "../common/snapshot.h"

#include "server_gameloop_message.h"

class ServerProtocol: public Protocol {
public:
    explicit ServerProtocol(Socket& skt): Protocol(skt) {}

    void recv_msg(uint8_t& command);


    void send_snapshot(std::shared_ptr<Snapshot> snapshot);

    // No se usan! Quitar despues de common/common_protocol.h
    void read_msg(void* msg) override;

    void send_msg(void* msg) override;
};

#endif  // SERVER_PROTOCOLO_H
