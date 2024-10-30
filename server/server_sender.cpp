#include "server_sender.h"

SenderThread::SenderThread(ServerProtocol& p): requester_queue(SENDER_QUEUE_SIZE), protocol(p) {}

void SenderThread::stop() {
    _is_alive = false;
    requester_queue.close();
}

Queue<DuckState>* SenderThread::get_queue() { return &this->requester_queue; }

void SenderThread::run() {
    try {
        while (this->_is_alive) {
            DuckState cmd = requester_queue.pop();
            protocol.send_msg(&cmd);
            std::cout << "SERVER: sended the duckstate\n";
        }
    } catch (const SocketWasCLosedException& e) {
        if (this->_is_alive) {
            syslog(LOG_ERR, "%s", e.what());
        }
        this->_is_alive = false;
    } catch (const std::exception& err) {
        if (this->_is_alive) {
            syslog(LOG_ERR, "%s", "Error occurred (SenderThread)\n");
        }
        this->_is_alive = false;
    }
}
