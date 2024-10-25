#ifndef SERVER_THREAD_RECEIVER_H
#define SERVER_THREAD_RECEIVER_H

#include "../common/common_queue.h"
#include "../common/common_thread.h"

#include "server_mensaje_gameloop.h"
#include "server_protocolo.h"

class ThreadReceiver: public Thread {
private:
    ServerProtocolo& protocolo;
    std::atomic<bool> is_alive;
    Queue<MensajeGameloop>& gameloop_queue;

public:
    explicit ThreadReceiver(ServerProtocolo& protocolo, Queue<MensajeGameloop>& gameloop_queue);

    void run() override;

    void detener_thread();

    bool esta_Vivo() const;
};

#endif  // SERVER_THREAD_RECEIVER_H
