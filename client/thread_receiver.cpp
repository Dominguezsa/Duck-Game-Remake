#include "thread_receiver.h"

#include <string>

ThreadReceiver::ThreadReceiver(ClientProtocol& protocol): protocolo(protocol), is_alive(true) {}


void ThreadReceiver::run() {
    try {
        while (is_alive) {
            std::vector<Duck> ducks = this->protocolo.receiveMessage();
            graphique_queue.push(ducks);
        }
    } catch (const std::exception& e) {
        this->is_alive = false;
    }
}


void ThreadReceiver::detener_thread() {
    this->is_alive = false;
    this->join();
}

bool ThreadReceiver::esta_Vivo() const { return is_alive; }
