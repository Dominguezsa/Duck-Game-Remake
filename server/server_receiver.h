#ifndef SERVER_THREAD_RECEIVER_H
#define SERVER_THREAD_RECEIVER_H

#include <syslog.h>
#include <cstdint>

#include "../common/common_queue.h"
#include "../common/common_thread.h"

#include "server_gameloop_message.h"
#include "server_protocol.h"

class ReceiverThread: public Thread {
private:
    Queue<GameloopMessage>& gameloop_queue;
    std::atomic<bool> is_alive;
    ServerProtocol& protocol;
    uint8_t session_id; // (temporalmente)

public:
    explicit ReceiverThread(Queue<GameloopMessage>& q, ServerProtocol& p,
                            uint8_t id);

    void run() override;

    void stop() override;
};

#endif  // SERVER_THREAD_RECEIVER_H
