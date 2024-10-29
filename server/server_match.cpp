#include "server_match.h"

Match::Match(unsigned int limit) : 
                                   gameloop_queue(),
                                   game(monitor, gameloop_queue), 
                                   status(MatchStatus::Waiting),
                                   accepting_players(true),
                                   player_limit(limit),
                                   player_count(0), 
                                   monitor() {}

bool Match::delete_player(uint8_t id) {
    bool deleted = monitor.delete_player(id);
    if (deleted) {
        player_count--;
        game.removePlayer(id);
    }
    return deleted;
}

bool Match::can_accept_players() const {
    return accepting_players;
}

void Match::add_player(Queue<DuckState> *q, uint8_t id) {
    if (accepting_players) {
        monitor.add_player(q, id);
        game.addPlayer(id);
        player_count++;
        this->accepting_players = status == MatchStatus::Waiting &&
                                  player_count < player_limit;
        if (!accepting_players) {
            initialize_game();
        }
    }
}

void Match::initialize_game() {
    if (player_count == player_limit) {
        status = MatchStatus::Playing;
        game.run();
    }
}

Queue<GameloopMessage>* Match::get_gameloop_queue() {
    return &gameloop_queue;
}
