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

void MatchStateMonitor::add_player(Queue<std::shared_ptr<Snapshot>>* q, uint8_t& id) {
    std::lock_guard<std::mutex> lock(data_mtx);

    if (accepting_players) {
        id = assigned_ids;
        requester_queues[id] = q;
        player_count++;
        std::cout << +player_count << " players in match\n";
        std::cout << +player_limit << " players limit\n";
        this->accepting_players = status == MatchStatus::Waiting && player_count < player_limit;
        if (!accepting_players) {
            std::cout << "Match is now playing\n";
            status = MatchStatus::Playing;
        }
        assigned_ids++;
    }
}

void MatchStateMonitor::stop_match() {
    std::lock_guard<std::mutex> lock(data_mtx);
    std::cout << "MatchStateMonitor::stop_match\n";
    status = MatchStatus::Finished;
    accepting_players = false;
    requester_queues.clear();
    std::cout << "end MatchStateMonitor::stop_match\n";
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
    if (status != MatchStatus::Waiting && player_count == 0) {
        status = MatchStatus::Finished;
    } else if (status == MatchStatus::Playing && player_count < 2) {
        status = MatchStatus::Finished;
    }
    return true;
}

void MatchStateMonitor::push_to_all(std::shared_ptr<Snapshot> duck_snapshot) {
    std::lock_guard<std::mutex> lock(data_mtx);
    for (auto it = requester_queues.begin(); it != requester_queues.end();) {
        try {
            it->second->push(duck_snapshot);
            it++;
        } catch (const ClosedQueue& e)  {
            std::cerr << "Queue closed, removing player " << int(it->first) << "\n";
            it = requester_queues.erase(it);
            player_count--;
            status = (player_count < 2) ? MatchStatus::Finished : MatchStatus::Playing;
        }
    }
}

bool MatchStateMonitor::match_is_finished() {
    std::cout << "MatchStateMonitor::match_is_finished\n";
    std::lock_guard<std::mutex> lock(data_mtx);
    std::cout << "MatchStateMonitor:: lock adquirido para el status\n";
    return status == MatchStatus::Finished;
}

bool MatchStateMonitor::match_is_playing() {
    std::lock_guard<std::mutex> lock(data_mtx);
    return status == MatchStatus::Playing;
}

bool MatchStateMonitor::waiting_for_players() {
    std::lock_guard<std::mutex> lock(data_mtx);
    return status == MatchStatus::Waiting;
}

void MatchStateMonitor::set_finished_status() {
    std::lock_guard<std::mutex> lock(data_mtx);
    std::cout << "MatchStateMonitor::set_finished_status\n";
    status = MatchStatus::Finished;
}