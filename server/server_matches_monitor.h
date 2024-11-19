#ifndef SERVER_MATCHES_MONITOR
#define SERVER_MATCHES_MONITOR

#include <list>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "../common/common_queue.h"

#include "server_match.h"

class MatchesMonitor {
private:
    std::unordered_map<std::string, std::shared_ptr<Match>> matches;
    std::mutex matches_mtx;

public:
    MatchesMonitor();

    /* Pos: Returns true if the match was successfully created.
            If that occurs, the player is added to the match and the created
            duck information is set to the duck_info struct.

            If the match could not be created, the function returns false.

    */
    bool create_match(std::string match_name, uint8_t player_limit, DuckIdentity& duck_info,
                      Queue<std::shared_ptr<Snapshot>>* q);

    /* Pos: Returns true if the player was successfully added to the match.
            If that occurs, the player is added to the match and the created
            duck information is set to the duck_info struct.

            If the player could not be added, the function returns false.
    */
    bool join_match(std::string match_name, DuckIdentity& duck_info,
                    Queue<std::shared_ptr<Snapshot>>* q);

    Queue<GameloopMessage>* get_match_queue(const std::string& match_name);

    std::list<std::string> get_available_match_names();

    void remove_finished_matches();

    void remove_all_matches();

    void disconnect_player(const std::string& match_name, const uint8_t& player_id);
};

#endif
