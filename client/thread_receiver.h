#ifndef THREAD_RECEIVER_H
#define THREAD_RECEIVER_H

#include <vector>

#include "../common/common_queue.h"
#include "../common/common_thread.h"
#include "../common/duck.h"
#include "../common/snapshot.h"
#include "../common/types/duck_state.h"

#include "protocol.h"

class ThreadReceiver: public Thread {
private:
    ClientProtocol& protocol;
    Queue<Snapshot>& ducks_snapshots_queue;

public:
    explicit ThreadReceiver(ClientProtocol& protocol, Queue<Snapshot>& graphique_queue);

    void run() override;

    void stop_thread();

    bool is_still_alive() const;
};

#endif  // THREAD_RECEIVER_H
