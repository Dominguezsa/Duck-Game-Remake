#ifndef SERVER_SENDER_H
#define SERVER_SENDER_H

#include <memory>
#include <vector>

#include <syslog.h>

#include "../common/common_liberror.h"
#include "../common/common_queue.h"
#include "../common/common_thread.h"
#include "../common/types/duck_state.h"
#include "../common/snapshot.h"

#include "server_protocol.h"

#define SENDER_QUEUE_SIZE 100

class SenderThread: public Thread {
private:
    Queue<Snapshot> requester_queue;
    ServerProtocol& protocol;
    Queue<std::shared_ptr<Snapshot>> duck_states_queue;

public:
    explicit SenderThread(ServerProtocol& p);
    Queue<std::shared_ptr<Snapshot>>* get_queue();
    void stop() override;
    void run() override;
};

#endif  // SERVER_SENDER_H
