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
    // cppcheck-suppress unusedStructMember
    std::unordered_map<uint8_t, std::unique_ptr<Duck>> ducks;
    // cppcheck-suppress unusedStructMember
    Queue<GameloopMessage>& message_queue;
    // cppcheck-suppress unusedStructMember
    std::atomic<bool> is_running;
    // cppcheck-suppress unusedStructMember
    uint8_t next_player_id;
    // cppcheck-suppress unusedStructMember
    uint16_t round_number;
    // cppcheck-suppress unusedStructMember
    std::unordered_map<uint8_t, uint16_t> victories;
    // cppcheck-suppress unusedStructMember
    std::vector<Platform> platforms;
    // cppcheck-suppress unusedStructMember
    MatchQueuesMonitor& monitor;

    // cppcheck-suppress unusedStructMember
    static constexpr double TICK_RATE = 60.0;
    // cppcheck-suppress unusedStructMember
    static constexpr double TICK_DURATION = 1.0 / TICK_RATE;
    // cppcheck-suppress unusedStructMember
    static constexpr uint16_t ROUNDS_PER_SET = 5;
    // cppcheck-suppress unusedStructMember
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
