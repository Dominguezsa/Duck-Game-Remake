#ifndef THREAD_SENDER_H
#define THREAD_SENDER_H

#include "../common/common_queue.h"
#include "../common/common_thread.h"


#include "protocol.h"

class ThreadSender: public Thread {
private:
    ClientProtocol& protocol;
    std::atomic<bool> is_alive;

public:
    explicit ThreadSender(ClientProtocol& protocol);

    void run() override;

    void stop_thread();

    bool is_still_alive() const;
};

#endif  // THREAD_SENDER_H