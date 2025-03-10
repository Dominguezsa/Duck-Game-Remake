#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "../common/common_protocol.h"
#include "../common/common_socket.h"
#include "../common/duck.h"
#include "../common/types/match_state.h"


class ClientProtocol: public Protocol {
public:
    explicit ClientProtocol(Socket& socket);

    void recv_platforms(std::vector<Platform>& platforms);
    void read_msg(void* msg) override;

    void send_msg(void* msg) override;

    ~ClientProtocol() override = default;
};

#endif  // CLIENT_PROTOCOL_H
