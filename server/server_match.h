#ifndef SERVER_MATCH_H
#define SERVER_MATCH_H

#include <cstdint>
#include <memory>
#include <vector>

#include "../common/snapshot.h"
#include "../common/types/match_state.h"
#include "types/match_status.h"

#include "server_game.h"
#include "server_match_state_monitor.h"

class Match {
private:
    Queue<GameloopMessage> gameloop_queue;
    Game game;
    MatchStateMonitor state_monitor;
    // Aún no formalizada la estructura del escenario.
    // Scenary scenary;

public:
    explicit Match(uint8_t limit);
    bool remove_player_if_in_match(const uint8_t& id);
    void add_player(Queue<std::shared_ptr<Snapshot>>* q, DuckIdentity& duck_info);
    Queue<GameloopMessage>* get_gameloop_queue();
    bool can_accept_players();
    bool is_finished();
    void stop_game();

private:
    void initialize_game();
};

#endif  // SERVER_MATCH_H
