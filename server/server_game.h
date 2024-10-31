#ifndef SERVER_GAME_H
#define SERVER_GAME_H

#include <atomic>
#include <chrono>
#include <memory>
#include <unordered_map>
#include <vector>

#include "../common/common_queue.h"
#include "../common/common_thread.h"
#include "../common/duck.h"
#include "../common/types/action_type.h"

#include "server_gameloop_message.h"
#include "server_match_queues_monitor.h"


class Game: public Thread {
private:
    struct Platform {
        // cppcheck-suppress unusedStructMember
        float x;
        // cppcheck-suppress unusedStructMember
        float y;
        // cppcheck-suppress unusedStructMember
        float width;
        // cppcheck-suppress unusedStructMember
        float height;
    };
    std::unordered_map<uint8_t, std::unique_ptr<Duck>> ducks;
    Queue<GameloopMessage>& message_queue;
    std::atomic<bool> is_running;
    uint8_t next_player_id;
    uint16_t round_number;
    std::unordered_map<uint8_t, uint16_t> victories;
    std::vector<Platform> platforms;
    MatchQueuesMonitor& monitor;

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
    bool checkPlatformCollision(const Position& duck_pos, float duck_width, float duck_height,
                                const Platform& platform);


public:
    explicit Game(MatchQueuesMonitor& monitor, Queue<GameloopMessage>& queue);

    void addPlayer(uint8_t player_id);
    void removePlayer(uint8_t player_id);
    void run() override;
    void stop() override;
    bool isRunning() const;

    ~Game() = default;
};

#endif  // SERVER_GAME_H
