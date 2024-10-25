#include "thread_receiver.h"

#include <string>

ThreadReceiver::ThreadReceiver(ClientProtocol& protocol, Queue<std::vector<Duck>>& graphique_queue):
    protocol(protocol), is_alive(true), graphique_queue(graphique_queue) {}


void ThreadReceiver::run() {
    try {
        while (is_alive) {
            std::vector<Duck> ducks = this->protocol.receiveMessage();
            graphique_queue.push(ducks);
        }
    } catch (const std::exception& e) {
        this->is_alive = false;
    }
}


void ThreadReceiver::stop_thread() {
    this->is_alive = false;
    this->join();
}

bool ThreadReceiver::is_still_alive() const { return is_alive; }
