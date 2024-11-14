#include "server_sender.h"

#include <memory>
#include <vector>
#include <iostream>
SenderThread::SenderThread(ServerProtocol& p, Queue<std::shared_ptr<std::vector<DuckState>>>& q):
        protocol(p), client_queue(q) {}

void SenderThread::join() {
    clear_queue();
    Thread::join();
}

void SenderThread::clear_queue() {
    bool is_empty = false;
    do {
        std::shared_ptr<std::vector<DuckState>> e;
        is_empty = client_queue.try_pop(e) == false;
    } while (!is_empty);
}

void SenderThread::run() {
    try {
        while (this->_is_alive) {
            std::shared_ptr<std::vector<DuckState>> snapshot = client_queue.pop();
            std::cout << "sending duck states\n";
            protocol.send_duck_states(snapshot);
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
