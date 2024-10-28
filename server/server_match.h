#ifndef SERVER_MATCH_H
#define SERVER_MATCH_H

#include <cstdint>
#include "server_match_queues_monitor.h"
#include "types/match_status.h"
#include "server_game.h"

class Match {
    private:
        // Creeria que el ownership de esta queue tiene que ser del Game.
        // Lo dejo asi para que compile.
        Queue<GameloopMessage> gameloop_queue;

        Game game;
        MatchStatus status;
        bool accepting_players;
        unsigned int player_limit;
        unsigned int player_count;
        MatchQueuesMonitor monitor;
        // Aún no formalizada la estructura del escenario.
        // Scenary scenary;
    
    public:
        Match(unsigned int limit);
        bool delete_player(uint8_t id);
        void add_player(Queue<DuckState> *q, uint8_t id);
        void initialize_game();
        bool can_accept_players() const;
};

#endif // SERVER_MATCH_H