#include "server_thread_receiver.h"

#include <string>
#include <vector>

ThreadReceiver::ThreadReceiver(ServerProtocol& protocolo, Queue<MensajeGameloop>& gameloop_queue):
        protocolo(protocolo), is_alive(true), gameloop_queue(gameloop_queue) {}


void ThreadReceiver::run() {
    try {
        while (is_alive) {
            //MensajeGameloop msj = this->protocolo.recibirMensaje();
            //gameloop_queue.push(msj);
        }
    } catch (const std::exception& e) {
        this->is_alive = false;
    }
}


void ThreadReceiver::detener_thread() {
    this->is_alive = false;
    this->join();
}

bool ThreadReceiver::is_still_alive() const { return is_alive; }
