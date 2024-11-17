#include "server_sender.h"

#include <memory>
#include <vector>

SenderThread::SenderThread(ServerProtocol& p): requester_queue(SENDER_QUEUE_SIZE), protocol(p) {}

void SenderThread::stop() {
    _is_alive = false;
    requester_queue.close();
}

Queue<std::shared_ptr<std::vector<DuckState>>>* SenderThread::get_queue() {
    return &this->duck_states_queue;
}

void SenderThread::run() {
    try {
        while (this->_is_alive) {
            std::shared_ptr<Snapshot> snapshot = duck_states_queue.pop();
            protocol.send_snapshot(snapshot);
            // std::cout << "SERVER: sended the duckstate\n";
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
