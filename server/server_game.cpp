#include "server_game.h"

#include <cstdint>
#include <iostream>

enum Directions : const uint8_t { LEFT, RIGHT, UP, DOWN };

Game::Game(MatchStateMonitor& monitor, Queue<GameloopMessage>& queue):
        message_queue(queue),
        is_running(false),
        next_player_id(0),
        round_number(0),
        monitor(monitor) {}

void Game::addPlayer(DuckIdentity& duck_info) {
    Position initial_pos{100 + (duck_info.id * 100), 100};  // Example starting positions
    Weapon initial_weapon(WeaponType::NoneType, 0);
    ducks[duck_info.id] = std::make_unique<Duck>(duck_info.id, 100, 1, initial_pos, initial_weapon);

    // TO-DO: Asignarle al Duck creado su nombre (i.e. el valor de duck_info.name)
    //        En el commit de este comentario aun no se implentó un metodo set_name()
    //        en la clase Duck.

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

void Game::handlePlayerAction(const GameloopMessage& msg) {
    auto it = ducks.find(msg.player_id);
    if (it == ducks.end())
        return;

    Duck* duck = it->second.get();
    // Ya esto se está haciendo medio inmanejable, deberiamos ya ver de hacer un double dispatch
    // (lo mismo en el cliente)
    switch (msg.action) {
        case MOVE_RIGHT_KEY_DOWN:
            duck->move_to(RIGHT);
            break;
        case MOVE_RIGHT_KEY_UP:
            duck->stop_running();
            break;
        case MOVE_LEFT_KEY_DOWN:
            duck->move_to(LEFT);
            break;
        case MOVE_LEFT_KEY_UP:
            duck->stop_running();
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
        case LOOKING_RIGHT_KEY_DOWN:
            duck->look_to(RIGHT);
            break;
        case LOOKING_LEFT_KEY_DOWN:
            duck->look_to(LEFT);
            break;
        case LOOKING_UP_KEY_DOWN:
            duck->look_to(UP);
            break;
        case LOOKING_DOWN_KEY_DOWN:
            duck->look_to(DOWN);
            break;
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
    const float gravity = 0.5f;
    const float flutter_force = -0.3f;
    const float max_fall_speed = 15.0f;
    const float duck_width = 64.0f;
    const float duck_height = 64.0f;
    const float ground_level = 700.0f - duck_height;
    const float move_speed = 5.0f;

    std::shared_ptr<std::vector<DuckState>> duck_states =
            std::make_shared<std::vector<DuckState>>();

    for (auto& duck_pair: ducks) {
        Duck* duck = duck_pair.second.get();

        // Store previous position for collision resolution
        float previous_y = duck->position.y;
        float previous_x = duck->position.x;

        // if (duck->duck_id == 1) {
        //     std::cout << "Duck 1 vertical velocity: " << duck->vertical_velocity << std::endl;
        // }

        // Apply gravity if in air
        if (duck->in_air) {
            duck->vertical_velocity += gravity;

            // Apply flutter effect if jumping is held
            if (duck->is_gliding) {
                duck->vertical_velocity += flutter_force;
                if (duck->vertical_velocity > 3.0f) {
                    duck->vertical_velocity = 3.0f;
                }
            }
        }

        // Limit fall speed
        if (duck->vertical_velocity > max_fall_speed) {
            duck->vertical_velocity = max_fall_speed;
        }

        // Update horizontal position
        if (duck->is_running) {
            // std::cout << "Duck be running\n";
            if (duck->looking == 1) {
                // duck->position.x += move_speed;
                double actual_pos = duck->position.x;
                double new_pos = actual_pos + move_speed;
                duck->position.x = new_pos;
                // std::cout << "New position is: " << duck->position.x << std::endl;
            } else if (duck->looking == 0) {
                duck->position.x -= move_speed;
                // std::cout << "New position is: " << duck->position.x << std::endl;
            }
        }

        // Apply vertical velocity
        duck->position.y += duck->vertical_velocity;

        // Check platform collisions
        for (const auto& platform: platforms) {
            if (checkPlatformCollision(duck->position, duck_width, duck_height, platform)) {
                if (duck->vertical_velocity > 0 && previous_y + duck_height <= platform.y) {
                    // Landing on platform
                    // Small adjustment to avoid a horizontal collision with the platform
                    duck->position.y = platform.y - duck_height - 2;
                    duck->vertical_velocity = 0;
                    duck->in_air = false;
                    duck->is_jumping = false;
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
        } else if (duck->position.x > 1200 - duck_width) {
            duck->position.x = 1200 - duck_width;
        }

        // Update duck state
        DuckState state(duck->duck_id, duck->life_points, duck->looking, duck->position,
                        duck->is_alive ? 1 : 0, duck->is_running ? 1 : 0, duck->is_jumping ? 1 : 0,
                        duck->is_gliding ? 1 : 0, duck->is_falling ? 1 : 0,
                        duck->is_ducking ? 1 : 0, duck->is_shooting ? 1 : 0,
                        duck->helmet_on ? 1 : 0, duck->armor_on ? 1 : 0, duck->in_air ? 1 : 0,
                        duck->vertical_velocity, duck->weapon.getType());

        duck->update_state(state);
        duck_states->push_back(state);
    }
    monitor.push_to_all(duck_states);
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
        Weapon initial_weapon(WeaponType::NoneType, 0);
        duck_pair.second =
                std::make_unique<Duck>(duck_pair.first, 100, 1, initial_pos, initial_weapon);
    }

    if (round_number % ROUNDS_PER_SET == 0) {
        if (checkGameEnd()) {
            stop();
        }
    }
}

bool Game::checkGameEnd() {
    // Por ahora tiro un suppress, en teoría debería usar un std::of_any() pero después vemos
    for (const auto& victory_pair: victories) {
        if (victory_pair.second >= VICTORIES_TO_WIN) {
            uint16_t max_victories = victory_pair.second;
            uint16_t next_max = 0;

            // Check if there's a tie
            for (const auto& other_pair: victories) {
                if (other_pair.first != victory_pair.first && other_pair.second > next_max) {
                    // Y acá tira de usar std::all_of, any_of o none_of ¿? el cppcheck
                    // cppcheck-suppress useStlAlgorithm
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

    while (is_running) {
        double start_time = getCurrentTime();

        // Process all pending messages
        GameloopMessage msg(0, 0);
        while (message_queue.try_pop(msg)) {
            handlePlayerAction(msg);
        }
        // std::cout << "Updating state\n";
        updateGameState();
        checkRoundEnd();


        double end_time = getCurrentTime();
        rateController(start_time, end_time);
    }
}

void Game::stop() { is_running = false; }

bool Game::isRunning() const { return is_running; }
