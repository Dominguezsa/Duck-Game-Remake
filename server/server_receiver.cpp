#include "server_receiver.h"

ReceiverThread::ReceiverThread(Queue<GameloopMessage>& q, ServerProtocol& p, uint8_t id):
        gameloop_queue(q), protocol(p), client_id(id) {}

void ReceiverThread::run() {
    GameloopMessage msg;
    msg.player_id = client_id;
    try {
        while (this->_is_alive) {
            protocol.recv_msg(msg.action);
            gameloop_queue.push(msg);
        }
    } catch (std::exception& e) {
        // If an exception is thrown in this while loop, that means the client
        // has disconnected from the server or the match is finished.
        std::cout << "Client disconnected : Printed on receiverThread\n";
        this->_is_alive = false;
    }
}

void ReceiverThread::stop() { this->_is_alive = false; }
