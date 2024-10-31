#ifndef SERVER_USER_CLIENT_H
#define SERVER_USER_CLIENT_H

#include <cstdint>
#include <memory>
#include <vector>

#include "../common/common_protocol.h"
#include "../common/common_socket.h"

#include "server_receiver.h"
#include "server_sender.h"

class UserClient {
private:
    // cppcheck-suppress unusedStructMember
    uint8_t id;
    Socket skt;
    ServerProtocol protocol;
    ReceiverThread receiver;
    SenderThread sender;

public:
    UserClient(Queue<GameloopMessage>& gameloop_queue, Socket&& skt, uint8_t id);

    bool is_alive();

    uint8_t get_id() const;

    Queue<std::shared_ptr<std::vector<DuckState>>>* get_queue();

    void end_communication();

    ~UserClient() = default;
};

#endif  // SERVER_USER_CLIENT_H
