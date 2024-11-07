#include "server_matches_monitor.h"

MatchesMonitor::MatchesMonitor() : matches(), matches_mtx() {}

bool MatchesMonitor::create_match(std::string match_name, uint8_t player_limit,
                                  uint8_t player_id,
                                  Queue<std::shared_ptr<std::vector<DuckState>>>* q) {
    std::lock_guard<std::mutex> lock(matches_mtx);
    
    bool match_exists = matches.find(match_name) != matches.end();
    bool player_limit_valid = player_limit > 0;
    if (match_exists || !player_limit_valid) {
        return false;
    }
    matches[match_name] = std::make_shared<Match>(player_limit);
    matches[match_name]->add_player(q, player_id);
    return true;
}

bool MatchesMonitor::join_match(std::string match_name, uint8_t player_id,
                                Queue<std::shared_ptr<std::vector<DuckState>>>* q) {
    std::lock_guard<std::mutex> lock(matches_mtx);
    
    bool match_exists = matches.find(match_name) != matches.end();
    bool player_limit_reached = !matches[match_name]->can_accept_players();
    if (!match_exists || player_limit_reached) {
        return false;
    }
    matches[match_name]->add_player(q, player_id);
    return true;
}

Queue<GameloopMessage>* MatchesMonitor::get_match_queue(std::string match_name) {
    std::lock_guard<std::mutex> lock(matches_mtx);
    return matches[match_name]->get_gameloop_queue();
}

std::vector<std::string> MatchesMonitor::get_available_match_names() {
    std::lock_guard<std::mutex> lock(matches_mtx);
    std::vector<std::string> available_matches;
    for (auto& match : matches) {
        if (match.second->can_accept_players()) {
            available_matches.push_back(match.first);
        }
    }
    return available_matches;
}

void MatchesMonitor::disconnect_player(uint8_t player_id) {
    std::lock_guard<std::mutex> lock(matches_mtx);
    for (auto& match : matches) {
        if (match.second->remove_player_if_in_match(player_id)) {
            return;
        }
    }
}
