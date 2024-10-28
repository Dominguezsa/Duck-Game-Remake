#ifndef MATCH_QUEUE_MONITOR_H
#define MATCH_QUEUE_MONITOR_H

#include <cstdint>
#include <unordered_map>
#include <mutex>

#include "../common/common_queue.h"
#include "../common/types/duck_state.h"

class MatchQueuesMonitor {
    private:
        std::mutex queues_mtx;
        std::unordered_map<uint8_t, Queue<DuckState>*> requester_queues;
    public:
        MatchQueuesMonitor();

        void add_player(Queue<DuckState> *q, uint8_t id);

        /* Pos: Returns true if the player was found and deleted,
                false otherwise. */
        bool delete_player(uint8_t id);

        // Pos: Broadcasts the current state of the duck to all players.
        void push_to_all(DuckState &duck_snapshot);
};

#endif // MATCH_QUEUE_MONITOR_H