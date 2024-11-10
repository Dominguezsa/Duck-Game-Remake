#include "server_matches_monitor.h"

MatchesMonitor::MatchesMonitor(): matches(), matches_mtx() {}

bool MatchesMonitor::create_match(std::string match_name, uint8_t player_limit,
                                  DuckIdentity& duck_info,
                                  Queue<std::shared_ptr<std::vector<DuckState>>>* q) {
    std::lock_guard<std::mutex> lock(matches_mtx);

    bool match_exists = matches.find(match_name) != matches.end();
    bool player_limit_valid = player_limit > 0;
    if (match_exists || !player_limit_valid) {
        return false;
    }
    matches[match_name] = std::make_shared<Match>(player_limit);
    matches[match_name]->add_player(q, duck_info);
    return true;
}

bool MatchesMonitor::join_match(std::string match_name, DuckIdentity& duck_info,
                                Queue<std::shared_ptr<std::vector<DuckState>>>* q) {
    std::lock_guard<std::mutex> lock(matches_mtx);

    bool match_exists = matches.find(match_name) != matches.end();
    bool player_limit_reached = !matches[match_name]->can_accept_players();
    if (!match_exists || player_limit_reached) {
        return false;
    }
    matches[match_name]->add_player(q, duck_info);
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
    std::lock_guard<std::mutex> lock(matches_mtx);
    for (auto it = matches.begin(); it != matches.end();) {
        Match* match = it->second.get();
        if (match->is_finished()) {
            match->stop_game();
            matches.erase(it);
        } else {
            it++;
        }
    }
}

void MatchesMonitor::remove_all_matches() {
    std::lock_guard<std::mutex> lock(matches_mtx);
    for (auto& match: matches) {
        match.second->stop_game();
    }
    matches.clear();
}
