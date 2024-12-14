#include "server_match.h"

#include <chrono>
#include <memory>
#include <thread>
#include <vector>

#define MAX_SIZE_QUEUE 50

Match::Match(uint8_t limit, const MapInfo& map_info):
        gameloop_queue(MAX_SIZE_QUEUE),
        game(state_monitor, gameloop_queue, map_info),
        state_monitor(limit),
        map_info(map_info) {}

bool Match::remove_player_if_in_match(const uint8_t& id) {
    bool removed = state_monitor.remove_player_if_present(id);
    if (removed) {
        game.removePlayer(id);
    }
    return removed;
}

bool Match::can_accept_players() { return state_monitor.status == MatchStatus::Waiting; }

void Match::add_player(Queue<std::shared_ptr<Snapshot>>* q, DuckIdentity& duck_info) {
    state_monitor.add_player(q, duck_info.id);
    game.addPlayer(duck_info, map_info);
    if (state_monitor.status == MatchStatus::Playing) {
        std::cout << "Match NOW PLAYING\n";
        initialize_game();
    }
}

void Match::initialize_game() {
    game.start();

    // state_monitor.end_game();
}

Queue<GameloopMessage>* Match::get_gameloop_queue() { return &gameloop_queue; }


bool Match::is_finished() {
    if (state_monitor.status == MatchStatus::Finished) {
        return true;
    }
    if (state_monitor.status == MatchStatus::Playing && !game.is_alive()) {
        return true;
    }
    return false;
}

void Match::stop_game() {
    game.stop();
    game.join();
    gameloop_queue.close();
    state_monitor.stop_match();
}
