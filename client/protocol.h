#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "../common/common_protocol.h"
#include "../common/common_socket.h"
#include "../common/duck.h"


class ClientProtocol: public Protocol {
public:
    explicit ClientProtocol(Socket& socket);

    std::vector<Duck> receiveMessage();

    void recv_player_amount(uint8_t& player_amount);

    void read_msg(void* msg) override;

    void send_msg(void* msg) override;

    ~ClientProtocol() override = default;
};

#endif  // CLIENT_PROTOCOL_H
