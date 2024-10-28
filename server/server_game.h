#ifndef SERVER_GAME_H
#define SERVER_GAME_H

#include <vector>
#include <unordered_map>
#include <memory>
#include <atomic>
#include <chrono>
#include <thread>
#include "../common/common_queue.h"
#include "../common/duck.h"
#include "server_gameloop_message.h"
#include "../common/types/action_type.h"

class Game {
private:
    std::unordered_map<uint8_t, std::unique_ptr<Duck>> ducks;
    Queue<GameloopMessage>& message_queue;
    std::atomic<bool> is_running;
    uint8_t next_player_id;
    uint16_t round_number;
    std::unordered_map<uint8_t, uint16_t> victories;
    
    static constexpr double TICK_RATE = 60.0;
    static constexpr double TICK_DURATION = 1.0 / TICK_RATE;
    static constexpr uint16_t ROUNDS_PER_SET = 5;
    static constexpr uint16_t VICTORIES_TO_WIN = 10;
    
    void handlePlayerAction(const GameloopMessage& msg);
    void updateGameState();
    void checkRoundEnd();
    void startNewRound();
    bool checkGameEnd();
    void rateController(double start, double finish);
    double getCurrentTime();

public:
    explicit Game(Queue<GameloopMessage>& queue);
    
    void addPlayer(uint8_t player_id);
    void removePlayer(uint8_t player_id);
    void run();
    void stop();
    bool isRunning() const;
    
    ~Game() = default;
};

#endif // SERVER_GAME_H
