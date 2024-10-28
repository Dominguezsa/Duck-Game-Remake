#ifndef SERVER_SENDER_H
#define SERVER_SENDER_H

#include <syslog.h>

#include "server_protocol.h"

#include "../common/common_thread.h"
#include "../common/common_queue.h"
#include "../common/common_liberror.h"
#include "../common/types/duck_state.h"

#define SENDER_QUEUE_SIZE 100

class SenderThread : public Thread {
private:
    Queue<DuckState> requester_queue;
    std::atomic<bool> is_alive;
    ServerProtocol &protocol;
public:
    explicit SenderThread(ServerProtocol &p);
    Queue<DuckState>* get_queue();
    void stop() override;
    void run() override;
};

#endif // SERVER_SENDER_H