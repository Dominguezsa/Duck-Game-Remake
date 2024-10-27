#ifndef SERVER_THREAD_RECEIVER_H
#define SERVER_THREAD_RECEIVER_H

#include "../common/common_queue.h"
#include "../common/common_thread.h"

#include "server_gameloop_message.h"
#include "server_protocolo.h"

struct MensajeGameloop {};

class ThreadReceiver: public Thread {
private:
    ServerProtocol& protocolo;
    std::atomic<bool> is_alive;
    Queue<MensajeGameloop>& gameloop_queue;

public:
    explicit ThreadReceiver(ServerProtocol& protocolo, Queue<MensajeGameloop>& gameloop_queue);

    void run() override;

    void detener_thread();

    bool is_still_alive() const;
};

#endif  // SERVER_THREAD_RECEIVER_H
