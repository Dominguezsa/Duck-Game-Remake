#include "server_client_session.h"

#include <utility>
ClientSession::ClientSession(Socket _skt, uint8_t _id, MatchesMonitor& monitor):
        id(_id),
        skt(std::move(_skt)),
        protocol(this->skt),
        sender(new SenderThread(protocol)),
        matches_monitor(monitor) {
    sender->start();
}

uint8_t ClientSession::get_id() const { return id; }

void ClientSession::end_communication() {
    // We kill the sender thread.
    sender->stop();
    sender->join();
    delete sender;

    // We disconnect the player from the any match it was in.
    matches_monitor.disconnect_player(id);

    // We close the communication channel.
    protocol.end_communication();
}
