#include "server_receiver.h"

ReceiverThread::ReceiverThread(Queue<GameloopMessage>& q, ServerProtocol& p, uint8_t id):
        gameloop_queue(q), protocol(p), client_id(id) {}

void ReceiverThread::run() {
    GameloopMessage msg;
    msg.player_id = client_id;
    try {
        while (this->_is_alive) {
            protocol.recv_duck_action(msg.action);
            gameloop_queue.push(msg);
        }
    } catch (const std::exception& e) {
        this->_is_alive = false;
    }
}

void ReceiverThread::stop() { this->_is_alive = false; }
