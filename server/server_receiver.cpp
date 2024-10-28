#include "server_receiver.h"

ReceiverThread::ReceiverThread(Queue<GameloopMessage>& q, ServerProtocol& p,
                               uint8_t id) :
                gameloop_queue(q),
                is_alive(true),
                protocol(p),
                session_id(id) {}

void ReceiverThread::run() {
    try {
        while (this->is_alive) {
            GameloopMessage msg;
            msg.player_id = this->session_id;
            protocol.recv_msg(msg);
            this->gameloop_queue.push(msg);
        }
    } catch (const SocketWasCLosedException& e) {
        if (this->_is_alive) {
            syslog(LOG_ERR, "%s", e.what());
        }
        this->is_alive = false;
    } catch (const std::exception& err) {
        if (this->_is_alive) {
            syslog(LOG_ERR, "%s", "Error ocurred (ReceiverThread)\n");
        }
        this->is_alive = false;
    }
}

void ReceiverThread::stop() {
    this->is_alive = true;
}
