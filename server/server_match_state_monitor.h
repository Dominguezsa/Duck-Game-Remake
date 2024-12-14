#ifndef MATCH_QUEUE_MONITOR_H
#define MATCH_QUEUE_MONITOR_H

#include <atomic>
#include <cstdint>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "../common/common_queue.h"
#include "../common/snapshot.h"
#include "../common/types/duck_state.h"
#include "types/match_status.h"

class MatchStateMonitor {
private:
    std::mutex data_mtx;
    const uint8_t player_limit;
    uint8_t assigned_ids;
    std::unordered_map<uint8_t, Queue<std::shared_ptr<Snapshot>>*> requester_queues;
    std::atomic<bool> accepting_players;
    std::atomic<uint8_t> player_count;
    std::atomic<MatchStatus> status;

public:
    explicit MatchStateMonitor(uint8_t limit);

    /* Pos: Adds a player to the queue and increments the player count.
            If the player limit is reached, the match status changes to playing.
            Assigns the player id to the id reference.
    */
    void add_player(Queue<std::shared_ptr<Snapshot>>* q, uint8_t& id);

    // Pos: Removes the player from the queue and decrements the player count.
    bool remove_player_if_present(const uint8_t& id);

    // Pos: Broadcasts the current state of the duck to all players.
    void push_to_all(std::shared_ptr<Snapshot> duck_snapshot);

    bool match_is_finished();

    bool match_is_playing();

    bool waiting_for_players();

    void stop_match();

    void set_finished_status();
};

#endif  // MATCH_QUEUE_MONITOR_H
