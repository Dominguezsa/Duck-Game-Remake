#ifndef SERVER_MATCHES_MONITOR
#define SERVER_MATCHES_MONITOR

#include <mutex>
#include <unordered_map>
#include <string>

#include "../common/common_queue.h"
#include "server_match.h"

class MatchesMonitor {
    private:
        std::unordered_map<std::string, std::shared_ptr<Match>> matches;
        std::mutex matches_mtx;

    public:
        MatchesMonitor();

        bool create_match(std::string match_name, uint8_t player_limit, uint8_t player_id,
                          Queue<std::shared_ptr<std::vector<DuckState>>>* q);
        
        bool join_match(std::string match_name, uint8_t player_id,
                        Queue<std::shared_ptr<std::vector<DuckState>>>* q);

        Queue<GameloopMessage>* get_match_queue(std::string match_name);

        std::vector<std::string> get_available_match_names();

        void remove_finished_matches();

        void remove_all_matches();

        void disconnect_player(uint8_t player_id);
};

#endif