#ifndef SERVER_GAME_H
#define SERVER_GAME_H

#include <atomic>
#include <chrono>
#include <map>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../common/bullet.h"
#include "../common/common_queue.h"
#include "../common/common_thread.h"
#include "../common/duck.h"
#include "../common/types/action_type.h"
#include "../common/types/match_state.h"

#include "PlayerActionHandler.h"
#include "server_gameloop_message.h"
#include "server_match_state_monitor.h"


class Game: public Thread {
private:
    std::unordered_map<uint8_t, std::unique_ptr<Duck>> ducks;
    std::vector<Weapon> weapons;
    Queue<GameloopMessage>& message_queue;
    std::atomic<bool> is_running;
    uint8_t next_player_id;
    uint16_t round_number;
    std::unordered_map<uint8_t, uint16_t> victories;
    std::vector<Platform> platforms;
    MatchStateMonitor& monitor;
    PlayerActionHandler action_handler;
    std::map<std::pair<uint32_t, uint8_t>, Bullet> bullets_by_id;  // son las balas que están
    // vivas en el juego, se guardan por id de la bala específica y el id del arma que la
    // disparó para ver los sprites a usar
    uint32_t next_bullet_id;

    static constexpr double TICK_RATE = 100.0;
    static constexpr double TICK_DURATION = 1.0 / TICK_RATE;
    static constexpr uint16_t ROUNDS_PER_SET = 5;
    static constexpr uint16_t VICTORIES_TO_WIN = 10;

    void updateGameState();
    void updateDuck(Duck* duck, std::shared_ptr<std::vector<DuckState>>& duck_states);
    void checkRoundEnd();
    void startNewRound();
    bool checkGameEnd();
    void rateController(double start, double finish);
    double getCurrentTime();
    bool checkPlatformCollision(const Position& duck_pos, float duck_width, float duck_height,
                                const Platform& platform);


public:
    explicit Game(MatchStateMonitor& monitor, Queue<GameloopMessage>& queue);

    /* Pos: Adds a player to the game and initializes its duck.
            The duck's initial position is set to the initial_pos_x and initial_pos_y
            fields of the duck_info struct.
    */
    void addPlayer(DuckIdentity& duck_info);

    void removePlayer(uint8_t player_id);
    void run() override;
    void stop() override;
    bool isRunning() const;

    ~Game() = default;
};

#endif  // SERVER_GAME_H
