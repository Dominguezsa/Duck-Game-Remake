#ifndef THREAD_RECEIVER_H
#define THREAD_RECEIVER_H

#include <vector>

#include "../common/common_queue.h"
#include "../common/common_thread.h"

#include "client_protocol.h"

class ThreadReceiver: public Thread {
private:
    ServerProtocolo& protocol;
    std::atomic<bool> is_alive;
    Queue<std::vector<Duck>>& graphique_queue;

public:
    explicit ThreadReceiver(ClientProtocol& protocol);

    void run() override;

    void stop_thread();

    bool is_still_alive() const;
};

#endif  // THREAD_RECEIVER_H
