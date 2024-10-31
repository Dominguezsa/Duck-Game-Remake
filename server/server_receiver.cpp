#include "server_receiver.h"

ReceiverThread::ReceiverThread(Queue<GameloopMessage>& q, ServerProtocol& p, uint8_t id):
        gameloop_queue(q), protocol(p), session_id(id) {}

void ReceiverThread::run() {
    try {
        while (this->_is_alive) {
            GameloopMessage msg;
            msg.player_id = this->session_id;
            protocol.recv_msg(msg.action);
            this->gameloop_queue.push(msg);
        }
    } catch (const SocketWasCLosedException& e) {
        if (this->_is_alive) {
            syslog(LOG_ERR, "%s", e.what());
        }
        this->_is_alive = false;
    } catch (const std::exception& err) {
        if (this->_is_alive) {
            syslog(LOG_ERR, "%s", "Error ocurred (ReceiverThread)\n");
        }
        this->_is_alive = false;
    }
}

void ReceiverThread::stop() { this->_is_alive = true; }
