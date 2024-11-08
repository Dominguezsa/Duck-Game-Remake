#ifndef MATCH_QUEUE_MONITOR_H
#define MATCH_QUEUE_MONITOR_H

#include <atomic>
#include <cstdint>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "../common/common_queue.h"
#include "../common/types/duck_state.h"
#include "types/match_status.h"

class MatchStateMonitor {
private:
    std::mutex data_mtx;
    const uint8_t player_limit;
    std::unordered_map<uint8_t, Queue<std::shared_ptr<std::vector<DuckState>>>*> requester_queues;
    std::atomic<bool> accepting_players;
    std::atomic<uint8_t> player_count;
    std::atomic<MatchStatus> status;

public:
    explicit MatchStateMonitor(uint8_t limit);

    /* Pos: Adds a player to the queue and increments the player count.
            If the player limit is reached, the match status changes to playing.
    */
    void add_player(Queue<std::shared_ptr<std::vector<DuckState>>>* q, uint8_t id);

    // Pos: Removes the player from the queue and decrements the player count.
    bool remove_player_if_present(uint8_t id);

    // Pos: Broadcasts the current state of the duck to all players.
    void push_to_all(std::shared_ptr<std::vector<DuckState>> duck_snapshot);

    bool playing_status();

    bool finished_status();

    void stop_match();
};

#endif  // MATCH_QUEUE_MONITOR_H
