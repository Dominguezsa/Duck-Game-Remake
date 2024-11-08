#include "server_sender.h"

#include <memory>
#include <vector>

SenderThread::SenderThread(ServerProtocol& p): requester_queue(SENDER_QUEUE_SIZE), protocol(p) {}

void SenderThread::stop() {
    _is_alive = false;
}

void SenderThread::join() {
    requester_queue.close();
    Thread::join();
}

void SenderThread::restart() {
    start();
    clear_queue();
    run();
}

Queue<std::shared_ptr<std::vector<DuckState>>>* SenderThread::get_queue() {
    return &this->duck_states_queue;
}

void SenderThread::clear_queue() {
    bool is_empty = false;
    do {
        std::shared_ptr<std::vector<DuckState>> element;
        is_empty = !duck_states_queue.try_pop(element);
    } while (!is_empty);
}

void SenderThread::run() {
    try {
        while (this->_is_alive) {
            std::shared_ptr<std::vector<DuckState>> snapshot = duck_states_queue.pop();
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
