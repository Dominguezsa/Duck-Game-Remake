#include "thread_receiver.h"

#include <string>

ThreadReceiver::ThreadReceiver(ClientProtocol& protocol,
                               Queue<std::vector<DuckState>>& graphique_queue):
        protocol(protocol), is_alive(true), ducks_snapshots_queue(graphique_queue) {}


void ThreadReceiver::run() {
    try {
        while (is_alive) {
            std::vector<DuckState> duck_states;

            this->protocol.read_msg(&duck_states);
            // std::cout << "The new position of duck_1: " << +duck_states[0].duck_id << "is: " <<
            // duck_states[0].position.x << ", " << duck_states[0].position.y << std::endl;
            // std::cout << "The new position of duck_2: " << +duck_states[1].duck_id << "is: " <<
            // duck_states[1].position.x << ", " << duck_states[1].position.y << std::endl;
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
