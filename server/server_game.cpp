#include "server_game.h"

Game::Game(Queue<GameloopMessage>& queue) 
    : message_queue(queue), 
      is_running(false), 
      next_player_id(0),
      round_number(0) {}

void Game::addPlayer(uint8_t player_id) {
    Position initial_pos{100 + (player_id * 100), 100}; // Example starting positions
    Weapon initial_weapon("NoneType", 0);
    ducks[player_id] = std::make_unique<Duck>(player_id, 100, 1, initial_pos, initial_weapon);
}

void Game::removePlayer(uint8_t player_id) {
    ducks.erase(player_id);
    if (ducks.empty()) {
        stop();
    }
}

void Game::handlePlayerAction(const GameloopMessage& msg) {
    auto it = ducks.find(msg.player_id);
    if (it == ducks.end()) return;
    
    Duck* duck = it->second.get();
    
    switch (msg.action) {
        case MOVE_RIGHT_KEY_DOWN:
            duck->move_to(1);
            break;
        case MOVE_LEFT_KEY_DOWN:
            duck->move_to(0);
            break;
        case JUMP_KEY_DOWN:
            duck->jump(true);
            break;
        case JUMP_KEY_UP:
            duck->jump(false);
            break;
        case SHOOT_KEY_DOWN:
            duck->shoot(true);
            break;
        case SHOOT_KEY_UP:
            duck->shoot(false);
            break;
    }
}

void Game::updateGameState() {
    // Update physics, collisions, weapons, etc.
    for (auto& duck_pair : ducks) {
        Duck* duck = duck_pair.second.get();
        // Update duck physics
        // Check collisions
        // Handle weapon effects
    }
}

void Game::checkRoundEnd() {
    int alive_players = 0;
    uint8_t last_alive_id = 0;
    
    for (const auto& duck_pair : ducks) {
        if (duck_pair.second->is_alive) {
            alive_players++;
            last_alive_id = duck_pair.first;
        }
    }
    
    if (alive_players <= 1) {
        if (alive_players == 1) {
            victories[last_alive_id]++;
        }
        startNewRound();
    }
}

void Game::startNewRound() {
    round_number++;
    
    // Reset all ducks to initial positions
    for (auto& duck_pair : ducks) {
        Position initial_pos{100 + (duck_pair.first * 100), 100};
        Weapon initial_weapon("NoneType", 0);
        duck_pair.second = std::make_unique<Duck>(
            duck_pair.first, 100, 1, initial_pos, initial_weapon
        );
    }
    
    if (round_number % ROUNDS_PER_SET == 0) {
        if (checkGameEnd()) {
            stop();
        }
    }
}

bool Game::checkGameEnd() {
    for (const auto& victory_pair : victories) {
        if (victory_pair.second >= VICTORIES_TO_WIN) {
            uint16_t max_victories = victory_pair.second;
            uint16_t next_max = 0;
            
            // Check if there's a tie
            for (const auto& other_pair : victories) {
                if (other_pair.first != victory_pair.first && 
                    other_pair.second > next_max) {
                    next_max = other_pair.second;
                }
            }
            
            if (max_victories > next_max) {
                return true;  // We have a winner
            }
        }
    }
    return false;
}

double Game::getCurrentTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count() / 1000.0;
}

void Game::rateController(double start, double finish) {
    double elapsed = finish - start;
    double sleep_time = TICK_DURATION - elapsed;
    
    if (sleep_time > 0) {
        std::this_thread::sleep_for(
            std::chrono::duration<double>(sleep_time)
        );
    }
}

void Game::run() {
    is_running = true;
    
    while (is_running) {
        double start_time = getCurrentTime();
        
        // Process all pending messages
        GameloopMessage msg(0, 0);
        while (message_queue.try_pop(msg)) {
            handlePlayerAction(msg);
        }
        
        updateGameState();
        checkRoundEnd();
        
        double end_time = getCurrentTime();
        rateController(start_time, end_time);
    }
}

void Game::stop() {
    is_running = false;
}

bool Game::isRunning() const {
    return is_running;
}
