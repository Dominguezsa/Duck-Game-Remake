#ifndef SERVER_MATCH_H
#define SERVER_MATCH_H

#include <cstdint>

#include "types/match_status.h"

#include "server_game.h"
#include "server_match_queues_monitor.h"

class Match {
private:
    Queue<GameloopMessage> gameloop_queue;
    Game game;
    // cppcheck-suppress unusedStructMember
    MatchStatus status;
    // cppcheck-suppress unusedStructMember
    bool accepting_players;
    // cppcheck-suppress unusedStructMember
    unsigned int player_limit;
    // cppcheck-suppress unusedStructMember
    unsigned int player_count;
    // cppcheck-suppress unusedStructMember
    MatchQueuesMonitor monitor;
    // Aún no formalizada la estructura del escenario.
    // Scenary scenary;

public:
    explicit Match(unsigned int limit);
    bool delete_player(uint8_t id);
    void add_player(Queue<DuckState>* q, uint8_t id);
    void initialize_game();
    Queue<GameloopMessage>* get_gameloop_queue();
    bool can_accept_players() const;
    bool is_finished();
    void stop_game();
};

#endif  // SERVER_MATCH_H
