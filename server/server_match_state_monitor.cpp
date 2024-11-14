#include "server_match_state_monitor.h"

#include <iostream>

MatchStateMonitor::MatchStateMonitor(uint8_t limit):
        data_mtx(),
        player_limit(limit),
        assigned_ids(0),
        requester_queues(),
        accepting_players(true),
        player_count(0),
        status(MatchStatus::Waiting) {}

void MatchStateMonitor::add_player(Queue<std::shared_ptr<std::vector<DuckState>>>* q, uint8_t& id) {
    std::lock_guard<std::mutex> lock(data_mtx);

    if (accepting_players) {
        id = ++assigned_ids;
        requester_queues[id] = q;
        player_count++;
        std::cout << +player_count << " players in match\n";
        std::cout << +player_limit << " players limit\n";
        this->accepting_players = status == MatchStatus::Waiting && player_count < player_limit;
        if (!accepting_players) {
            std::cout << "Match is now playing\n";
            status = MatchStatus::Playing;
        }
    }
}

bool MatchStateMonitor::playing_status() { return status == MatchStatus::Playing; }

bool MatchStateMonitor::finished_status() { return status == MatchStatus::Finished; }

void MatchStateMonitor::stop_match() {
    std::lock_guard<std::mutex> lock(data_mtx);
    status = MatchStatus::Finished;
    accepting_players = false;
}

bool MatchStateMonitor::remove_player_if_present(const uint8_t& id) {
    std::lock_guard<std::mutex> lock(data_mtx);

    auto it = requester_queues.find(id);
    if (it == requester_queues.end()) {
        return false;
    }
    requester_queues.erase(it);
    player_count--;

    // If last player leaves, match is finished.
    if (status != MatchStatus::Finished && player_count == 0) {
        status = MatchStatus::Finished;
    }
    return true;
}

void MatchStateMonitor::push_to_all(std::shared_ptr<std::vector<DuckState>> duck_snapshot) {
    std::lock_guard<std::mutex> lock(data_mtx);
    for (auto& pair: requester_queues) {
        pair.second->push(duck_snapshot);
    }
}
