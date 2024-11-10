#ifndef SERVER_THREAD_RECEIVER_H
#define SERVER_THREAD_RECEIVER_H

#include <cstdint>

#include <syslog.h>

#include "../common/common_queue.h"
#include "../common/common_thread.h"

#include "server_gameloop_message.h"
#include "server_protocol.h"

class ReceiverThread: public Thread {
private:
    Queue<GameloopMessage>& gameloop_queue;
    ServerProtocol& protocol;
    uint8_t session_id;

public:
    explicit ReceiverThread(Queue<GameloopMessage>& q, ServerProtocol& p, uint8_t id);

    void run() override;

    void stop() override;
};

#endif  // SERVER_THREAD_RECEIVER_H
