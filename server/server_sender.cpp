#include "server_sender.h"

SenderThread::SenderThread(ServerProtocol& p, Queue<std::shared_ptr<Snapshot>>& q):
        protocol(p), client_queue(q) {}

void SenderThread::clear_queue() {
    try {
        bool is_empty = false;
        do {
            std::shared_ptr<Snapshot> e;
            is_empty = client_queue.try_pop(e) == false;
        } while (!is_empty);
    } catch (const ClosedQueue& e) {
        syslog(LOG_ERR, "%s", e.what());
        // ojito con cerrar syslog varias veces
        closelog();
    }
}

void SenderThread::stop() {
    this->_is_alive = false;
    // este clear_queue no hace falta
    clear_queue();
}

void SenderThread::run() {
    try {
        while (this->_is_alive) {
            std::shared_ptr<Snapshot> snapshot = client_queue.pop();
            protocol.send_snapshot(snapshot);
        }
    } catch (const SocketWasCLosedException& e) {
        if (this->_is_alive) {
            syslog(LOG_ERR, "%s", e.what());
            closelog();
        }
        this->_is_alive = false;
    } catch (const std::exception& err) {
        if (this->_is_alive) {
            syslog(LOG_ERR, "%s", "Error occurred (SenderThread)\n");
            closelog();
        }
        this->_is_alive = false;
    }
}
