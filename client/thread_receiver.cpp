#include "thread_receiver.h"

#include <string>

ThreadReceiver::ThreadReceiver(ClientProtocol& protocol, Queue<Snapshot>& graphique_queue):
        protocol(protocol), is_alive(true), ducks_snapshots_queue(graphique_queue) {}


void ThreadReceiver::run() {
    try {
        while (is_alive) {
            Snapshot snapShot;
            this->protocol.read_msg(&snapShot);
            // std::cout << snapShot.ducks.size() << " ducks in snapshot\n";
            // std::cout << "Theres this many bullets in a snapshot: " << snapShot.bullets.size() <<
            // std::endl; std::cout << "Theres this many weapons in a snapshot: " <<
            // snapShot.weapons.size() << std::endl;
            ducks_snapshots_queue.push(snapShot);
        }
    } catch (const std::exception& e) {
        this->is_alive = false;
    }
    std::cout << "RECEIVER: run function ended\n";
}


void ThreadReceiver::stop_thread() {
    this->is_alive = false;
    this->protocol.end_communication();
    this->join();
    std::cout << "RECEIVER: stop_thread function ended\n";
}

bool ThreadReceiver::is_still_alive() const { return is_alive; }
