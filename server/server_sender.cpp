#include "server_sender.h"

SenderThread::SenderThread(ServerProtocol &p) : 
              requester_queue(SENDER_QUEUE_SIZE),
              is_alive(true),
              protocol(p) {}

void SenderThread::stop() {
    is_alive = false;
    requester_queue.close();
}

Queue<DuckState>* SenderThread::get_queue() {
    return &this->requester_queue;
}

void SenderThread::run() {
    try {
        while (this->is_alive) {
            DuckState cmd = requester_queue.pop();
            protocol.send_msg(&cmd);
        }
    } catch (const SocketWasCLosedException& e) {
        if (this->is_alive) {
            syslog(LOG_ERR, "%s", e.what());
        }
        this->is_alive = false;
    } catch (const std::exception& err) {
        if (this->is_alive) {
            syslog(LOG_ERR, "%s", "Error occurred (SenderThread)\n");
        }
        this->is_alive = false;
    }
}
