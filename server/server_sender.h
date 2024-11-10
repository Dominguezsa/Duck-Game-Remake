#ifndef SERVER_SENDER_H
#define SERVER_SENDER_H

#include <memory>
#include <vector>

#include <syslog.h>

#include "../common/common_liberror.h"
#include "../common/common_queue.h"
#include "../common/common_thread.h"
#include "../common/types/duck_state.h"

#include "server_protocol.h"

#define SENDER_QUEUE_SIZE 100

class SenderThread: public Thread {
private:
    ServerProtocol& protocol;
    Queue<std::shared_ptr<std::vector<DuckState>>>& client_queue;

public:
    explicit SenderThread(ServerProtocol& p, Queue<std::shared_ptr<std::vector<DuckState>>>& q);
    void join() override;
    void run() override;

private:
    void clear_queue();
};

#endif  // SERVER_SENDER_H
