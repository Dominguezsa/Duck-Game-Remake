#include "server_match_queues_monitor.h"

MatchQueuesMonitor::MatchQueuesMonitor() : queues_mtx(), requester_queues() {}

void MatchQueuesMonitor::add_player(Queue<DuckState> *q, uint8_t id) {
    std::lock_guard<std::mutex> lock(queues_mtx);
    requester_queues[id] = q;
}

bool MatchQueuesMonitor::delete_player(uint8_t id) {
    std::lock_guard<std::mutex> lock(queues_mtx);
    bool deleted = requester_queues.erase(id) != 0;
    return deleted;
}

void MatchQueuesMonitor::push_to_all(DuckState &duck_snapshot) {
    std::lock_guard<std::mutex> lock(queues_mtx);
    for (auto &pair : requester_queues) {
        pair.second->push(duck_snapshot);
    }
}
