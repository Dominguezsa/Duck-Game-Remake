#include "thread_receiver.h"

#include <string>

ThreadReceiver::ThreadReceiver(ClientProtocol& protocol, Queue<Snapshot>& graphique_queue):
        protocol(protocol), ducks_snapshots_queue(graphique_queue) {}


void ThreadReceiver::run() {
    try {
        while (_keep_running) {
            Snapshot snapShot;
            this->protocol.read_msg(&snapShot);
            ducks_snapshots_queue.push(snapShot);
        }
    } catch (const std::exception& e) {
        this->_is_alive = false;
        ducks_snapshots_queue.close();
    }
}

void ThreadReceiver::receivePlatforms(std::vector<Platform>& platforms) {
    this->protocol.recv_platforms(platforms);
    Snapshot snapshot;
    for (auto plat: platforms) {
        snapshot.addPlatform(plat);
    }
}


void ThreadReceiver::stop_thread() {
    this->_is_alive = false;
    this->protocol.end_communication();
    this->join();
}

bool ThreadReceiver::is_still_alive() const { return _is_alive; }
