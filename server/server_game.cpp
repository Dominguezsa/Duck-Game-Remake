#include "server_game.h"

#include <cstdint>
#include <iostream>

#include "../common/snapshot.h"

enum Directions : const uint8_t { LEFT, RIGHT, UP, DOWN };

#define GRAVITY 0.5f
#define FLUTTER_FORCE -0.3f
#define MAX_FALL_SPEED 13.0f
#define MAX_FLUTTER_SPEED 3.0f
#define DUCK_WIDTH 64.0f
#define DUCK_HEIGHT 64.0f
// #define MOVE_SPEED 5.0f

#define GRAVITY 0.5f
#define FLUTTER_FORCE -0.3f
#define MAX_FALL_SPEED 13.0f
#define MAX_FLUTTER_SPEED 3.0f
#define DUCK_WIDTH 64.0f
#define DUCK_HEIGHT 64.0f
#define MOVE_SPEED 5.0f
#define STOP_SPEED 0.1f

#define MAX_HORIZONTAL_SPEED 5.0f

Game::Game(MatchStateMonitor& monitor, Queue<GameloopMessage>& queue):
        weapons({Weapon(0, "ak47", 30, 15, 20,
                        {20, 20})}),  // las weapons deberian estar en el yaml
        message_queue(queue),
        is_running(false),
        next_player_id(0),
        round_number(0),
        monitor(monitor),
        action_handler(ducks) {}

void Game::addPlayer(DuckIdentity& duck_info) {
    Position initial_pos{100 + (duck_info.id * 100), 100};  // Example starting position
    Weapon initial_weapon;
    ducks[duck_info.id] = std::make_unique<Duck>(duck_info.id, 100, 1, initial_pos, initial_weapon,
                                                 duck_info.name);

    platforms = {
            {0.0f, 350.0f, 600.0f, 32.0f},   // Left platform
            {600.0f, 450.0f, 600.0f, 32.0f}  // Right platform
    };
    duck_info.initial_pos_x = initial_pos.x;
    duck_info.initial_pos_y = initial_pos.y;
}


void Game::removePlayer(uint8_t player_id) {
    ducks.erase(player_id);
    if (ducks.empty()) {
        stop();
    }
}


bool Game::checkPlatformCollision(const Position& duck_pos, float duck_width, float duck_height,
                                  const Platform& platform) {
    // Define duck's collision box (slightly smaller than sprite for better gameplay)
    float duck_collision_x = duck_pos.x + duck_width / 4;
    float duck_collision_width = duck_width / 2;

    // Check for full collision box overlap
    bool vertical_overlap = (duck_pos.y + duck_height >= platform.y) &&
                            (duck_pos.y <= platform.y + platform.height);
    bool horizontal_overlap = (duck_collision_x + duck_collision_width >= platform.x) &&
                              (duck_collision_x <= platform.x + platform.width);

    return vertical_overlap && horizontal_overlap;
}

void Game::updateGameState() {
    // const float gravity = 0.5f;
    // const float FLUTTER_FORCE = -0.3f;
    // const float max_fall_speed = 15.0f;
    // const float duck_width = 64.0f;
    // const float duck_height = 64.0f;
    const float ground_level = 700.0f - DUCK_HEIGHT;
    // const float move_speed = 5.0f;

    std::shared_ptr<std::vector<DuckState>> duck_states =
            std::make_shared<std::vector<DuckState>>();

    std::shared_ptr<std::vector<Bullet>> bullets_in_game = std::make_shared<std::vector<Bullet>>();

    for (auto& duck_pair: ducks) {
        Duck* duck = duck_pair.second.get();

        // Store previous position for collision resolution
        float previous_y = duck->position.y;
        float previous_x = duck->position.x;

        // if (duck->duck_id == 1) {
        //     std::cout << "Duck 1 vertical velocity: " << duck->vertical_velocity << std::endl;
        // }


        if (duck->is_shooting && duck->weapon.ammo > 0) {
            if (duck->weapon.actual_cicle == 0) {
                Bullet bullet(duck->weapon.id, duck->position.x, duck->position.y + DUCK_HEIGHT / 2,
                              duck->looking == 1 ? 0 : M_PI, 10.0f, 0.0f, duck->looking == 1,
                              duck->weapon.damage);
                bullets_in_game->push_back(bullet);
                duck->weapon.ammo--;
                duck->weapon.actual_cicle++;
                std::cout << "se disparo una balabala\n";
            } else {
                if (duck->weapon.actual_cicle == duck->weapon.cicles_to_reshoot) {
                    duck->weapon.actual_cicle = 0;
                } else {
                    duck->weapon.actual_cicle++;
                }
            }
        }

        // Apply gravity if in air
        if (duck->in_air) {
            duck->vertical_velocity += GRAVITY;

            // Apply flutter effect if jumping is held
            if (duck->is_gliding) {
                duck->vertical_velocity += FLUTTER_FORCE;
                if (duck->vertical_velocity > MAX_FLUTTER_SPEED) {
                    duck->vertical_velocity = MAX_FLUTTER_SPEED;
                }
            }
        }

        // Limit fall speed
        if (duck->vertical_velocity > MAX_FALL_SPEED) {
            duck->vertical_velocity = MAX_FALL_SPEED;
        }

        // Update horizontal position
        if (duck->is_running) {
            // std::cout << "Duck be running\n";

            duck->horizontal_velocity += MOVE_SPEED;
            if (duck->horizontal_velocity > MAX_HORIZONTAL_SPEED) {
                duck->horizontal_velocity = MAX_HORIZONTAL_SPEED;
            }

            if (duck->looking == 1) {
                // duck->position.x += move_speed;
                double actual_pos = duck->position.x;
                double new_pos = actual_pos + duck->horizontal_velocity;
                duck->position.x = new_pos;
                // std::cout << "New position is: " << duck->position.x << std::endl;
            } else if (duck->looking == 0) {
                duck->position.x -= duck->horizontal_velocity;
                // std::cout << "New position is: " << duck->position.x << std::endl;
            }
        } else if (duck->is_sliding) {
            duck->horizontal_velocity -= STOP_SPEED;
            if (duck->horizontal_velocity < 0) {
                duck->horizontal_velocity = 0;
            }

            if (duck->looking == 1) {
                duck->position.x += duck->horizontal_velocity;
            } else if (duck->looking == 0) {
                duck->position.x -= duck->horizontal_velocity;
            }
        }

        // Apply vertical velocity
        duck->position.y += duck->vertical_velocity;

        // Check platform collisions
        for (const auto& platform: platforms) {
            if (checkPlatformCollision(duck->position, DUCK_WIDTH, DUCK_HEIGHT, platform)) {
                if (duck->vertical_velocity > 0 && previous_y + DUCK_HEIGHT <= platform.y) {
                    // Landing on platform
                    // Small adjustment to avoid a horizontal collision with the platform
                    duck->position.y = platform.y - DUCK_HEIGHT - 2;
                    duck->vertical_velocity = 0;
                    duck->in_air = false;
                    duck->is_jumping = false;
                    duck->is_gliding = false;
                } else if (duck->vertical_velocity < 0 &&
                           previous_y >= platform.y + platform.height) {
                    // Hitting platform from below
                    duck->position.y = platform.y + platform.height;
                    duck->vertical_velocity = 0;
                } else {
                    // Side collision
                    duck->position.x = previous_x;
                    // std::cout << "Im detecting a collision\n";
                }
            }
        }

        // Ground collision
        if (duck->position.y > ground_level) {
            duck->position.y = ground_level;
            duck->vertical_velocity = 0;
            duck->in_air = false;
        }

        // Screen boundaries
        if (duck->position.x < 0) {
            duck->position.x = 0;
        } else if (duck->position.x > 1200 - DUCK_WIDTH) {
            duck->position.x = 1200 - DUCK_WIDTH;
        }

        duck->is_falling = duck->vertical_velocity > 0;

        // Update duck state
        DuckState state(duck->name, duck->duck_id, duck->life_points, duck->looking, duck->position,
                        duck->is_alive ? 1 : 0, duck->is_running ? 1 : 0, duck->is_jumping ? 1 : 0,
                        duck->is_gliding ? 1 : 0, duck->is_falling ? 1 : 0,
                        duck->is_ducking ? 1 : 0, duck->is_shooting ? 1 : 0,
                        duck->is_sliding ? 1 : 0, duck->helmet_on ? 1 : 0, duck->armor_on ? 1 : 0,
                        duck->in_air ? 1 : 0, duck->vertical_velocity, duck->horizontal_velocity,
                        duck->weapon.getType());

        duck->update_state(state);
        duck_states->push_back(state);
    }
    Snapshot snapshot(*duck_states, *bullets_in_game, weapons);
    monitor.push_to_all(std::make_shared<Snapshot>(snapshot));
}

void Game::checkRoundEnd() {
    int alive_players = 0;
    uint8_t last_alive_id = 0;

    for (const auto& duck_pair: ducks) {
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
    for (auto& duck_pair: ducks) {
        Position initial_pos{100 + (duck_pair.first * 100), 100};
        Weapon initial_weapon;
        duck_pair.second = std::make_unique<Duck>(duck_pair.first, 100, 1, initial_pos,
                                                  initial_weapon, duck_pair.second->name);
    }

    if (round_number % ROUNDS_PER_SET == 0) {
        if (checkGameEnd()) {
            stop();
        }
    }
}

bool Game::checkGameEnd() {
    // No se y no entiendo por que mi cppcheck local se pone como loca de que debería usar
    // algoritmos de la librería estándar para esto aunque haga que el código sea horrible,
    // estoy hace horas tratando de que no pase pero ni idea por que carajo sigue igual, otro
    // dia se verá
    auto max_victory = std::max_element(
            victories.cbegin(), victories.cend(),
            [](const auto& lhs, const auto& rhs) { return lhs.second < rhs.second; });

    if (max_victory->second >= VICTORIES_TO_WIN) {
        auto next_max = std::any_of(victories.cbegin(), victories.cend(),
                                    [max_victory](const auto& pair) {
                                        return pair.first != max_victory->first &&
                                               pair.second > max_victory->second;
                                    }) ?
                                std::max_element(victories.cbegin(), victories.cend(),
                                                 [](const auto& lhs, const auto& rhs) {
                                                     return lhs.second < rhs.second;
                                                 })
                                        ->second :
                                0;

        return max_victory->second > next_max;
    }

    return false;
}

double Game::getCurrentTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::system_clock::now().time_since_epoch())
                   .count() /
           1000.0;
}

void Game::rateController(double start, double finish) {
    double elapsed = finish - start;
    double sleep_time = TICK_DURATION - elapsed;

    if (sleep_time > 0) {
        std::this_thread::sleep_for(std::chrono::duration<double>(sleep_time));
    }
}

void Game::run() {
    is_running = true;
    try {
        while (is_running) {
            double start_time = getCurrentTime();

            // Process all pending messages
            GameloopMessage msg(0, 0);
            while (message_queue.try_pop(msg)) {
                action_handler.process_player_action(msg);
                // handlePlayerAction(msg);
            }
            // std::cout << "Updating state\n";
            updateGameState();
            checkRoundEnd();


            double end_time = getCurrentTime();
            rateController(start_time, end_time);
        }
    } catch (ClosedQueue& e) {
        std::cerr << "Server interrupts game loop\n";
        stop();
    } catch (const std::exception& e) {
        std::cerr << "Error occurred during game loop: " << e.what() << std::endl;
        stop();
    }
}

void Game::stop() { is_running = false; }

bool Game::isRunning() const { return is_running; }
