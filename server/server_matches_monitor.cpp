#include "server_matches_monitor.h"

MatchesMonitor::MatchesMonitor(): matches(), matches_mtx() {}

bool MatchesMonitor::create_match(std::string match_name, uint8_t player_limit,
                                  DuckIdentity& duck_info, Queue<std::shared_ptr<Snapshot>>* q,
                                  MapInfo mapInfo) {
    std::lock_guard<std::mutex> lock(matches_mtx);

    bool match_exists = matches.find(match_name) != matches.end();
    bool player_limit_valid = player_limit > 0;
    if (match_exists || !player_limit_valid) {
        return false;
    }
    matches[match_name] = std::make_shared<Match>(player_limit, mapInfo);
    matches[match_name]->add_player(q, duck_info);
    return true;
}

bool MatchesMonitor::join_match(std::string match_name, DuckIdentity& duck_info,
                                Queue<std::shared_ptr<Snapshot>>* q) {

    std::lock_guard<std::mutex> lock(matches_mtx);
    bool match_exists = matches.find(match_name) != matches.end();
    bool player_limit_reached = !matches[match_name]->can_accept_players();
    if (!match_exists || player_limit_reached) {
        std::cout << "Match does not exist or is full\n";
        return false;
    }
    matches[match_name]->add_player(q, duck_info);
    std::cout << "Player added to match\n";
    return true;
}

Queue<GameloopMessage>* MatchesMonitor::get_match_queue(const std::string& match_name) {
    std::lock_guard<std::mutex> lock(matches_mtx);
    return matches[match_name]->get_gameloop_queue();
}

std::list<std::string> MatchesMonitor::get_available_match_names() {
    std::lock_guard<std::mutex> lock(matches_mtx);
    std::list<std::string> available_matches;
    for (auto& match: matches) {
        if (match.second->can_accept_players()) {
            available_matches.push_back(match.first);
        }
    }
    return available_matches;
}

void MatchesMonitor::disconnect_player(const std::string& match_name, const uint8_t& player_id) {
    std::lock_guard<std::mutex> lock(matches_mtx);
    matches[match_name]->remove_player_if_in_match(player_id);
}

void MatchesMonitor::remove_finished_matches() {
    std::cout << "Removing finished matches\n";
    std::lock_guard<std::mutex> lock(matches_mtx);
    std::cout << "Total matches: " << matches.size() << std::endl;
    for (auto it = matches.begin(); it != matches.end();) {
        Match* match = it->second.get();
        std::string name = it->first;
        std::cout << "Checking " << name << "match\n";
        if (match->is_finished()) {
            std::cout << "Stopping match\n";
            match->stop_game();
            it = matches.erase(it);
            std::cout << "end Stopping match\n";
        } else {
            it++;
        }
    }
    std::cout << "after end ->  total matches: " << matches.size() << std::endl;
    std::cout << "Done remove matches\n";
}

void MatchesMonitor::remove_all_matches() {
    std::lock_guard<std::mutex> lock(matches_mtx);
    for (auto& match: matches) {
        match.second->stop_game();
    }
    matches.clear();
}
