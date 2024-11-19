#include "thread_receiver.h"

#include <string>

ThreadReceiver::ThreadReceiver(ClientProtocol& protocol,
                               Queue<std::vector<DuckState>>& graphique_queue):
        protocol(protocol), is_alive(true), ducks_snapshots_queue(graphique_queue) {}


void ThreadReceiver::run() {
    try {
        // std::cout << "Este es el primer snapshot que recibí\n";
        while (is_alive) {
            std::vector<DuckState> duck_states;
            this->protocol.read_msg(&duck_states);
            // std::cout << duck_states.size() << " ducks in snapshot\n";
            ducks_snapshots_queue.push(duck_states);
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
