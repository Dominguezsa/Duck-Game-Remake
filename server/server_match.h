#ifndef SERVER_MATCH_H
#define SERVER_MATCH_H

#include <cstdint>
#include <memory>
#include <vector>

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
    void delete_player(uint8_t id);
    void add_player(Queue<std::shared_ptr<std::vector<DuckState>>>* q, uint8_t id);
    Queue<GameloopMessage>* get_gameloop_queue();
    bool can_accept_players();
    bool is_finished();
    void stop_game();

private:
    void initialize_game();
};

#endif  // SERVER_MATCH_H
