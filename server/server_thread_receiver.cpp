#include "server_thread_receiver.h"

#include <string>
#include <vector>

ThreadReceiver::ThreadReceiver(ServerProtocolo& protocolo, Queue<MensajeGameloop>& gameloop_queue):
        protocolo(protocolo), esta_vivo(true), gameloop_queue(gameloop_queue) {}


void ThreadReceiver::run() {
    try {
        while (esta_vivo) {
            MensajeGameloop msj = this->protocolo.recibirMensaje();
            gameloop_queue.push(msj);
        }
    } catch (const std::exception& e) {
        this->esta_vivo = false;
    }
}


void ThreadReceiver::detener_thread() {
    this->esta_vivo = false;
    this->join();
}

bool ThreadReceiver::esta_Vivo() const { return esta_vivo; }
