#include "server_match.h"

#include <memory>
#include <vector>

#define MAX_SIZE_QUEUE 50

Match::Match(uint8_t limit):
        gameloop_queue(MAX_SIZE_QUEUE), game(state_monitor, gameloop_queue), state_monitor(limit) {}

bool Match::remove_player_if_in_match(uint8_t id) {
    bool removed = state_monitor.remove_player_if_present(id);
    if (removed) {
        game.removePlayer(id);
    }
    return removed;
}

bool Match::can_accept_players() { return !state_monitor.playing_status(); }

void Match::add_player(Queue<std::shared_ptr<std::vector<DuckState>>>* q, uint8_t id) {
    state_monitor.add_player(q, id);
    game.addPlayer(id);
    if (state_monitor.playing_status()) {
        initialize_game();
    }
}

void Match::initialize_game() {
    game.start();
    game.run();
}

Queue<GameloopMessage>* Match::get_gameloop_queue() { return &gameloop_queue; }

bool Match::is_finished() { return state_monitor.finished_status(); }

void Match::stop_game() {
    game.stop();
    game.join();
    gameloop_queue.close();
    state_monitor.stop_match();
}
