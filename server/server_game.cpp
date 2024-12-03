#include "server_game.h"

#include <algorithm>
#include <cstdint>
#include <iostream>

#include <unistd.h>

#include "../common/snapshot.h"
#include "../editor/image_type.h"

#include "weapon_handler.h"


#define GRAVITY 0.3f
#define FLUTTER_FORCE -0.3f
#define MAX_FALL_SPEED 13.0f
#define MAX_FLUTTER_SPEED 3.0f
#define DUCK_WIDTH 64.0f
#define DUCK_HEIGHT 64.0f
// #define MOVE_SPEED 5.0f

#define FLUTTER_FORCE -0.3f
#define MAX_FALL_SPEED 13.0f
#define MAX_FLUTTER_SPEED 3.0f
#define DUCK_WIDTH 64.0f
#define DUCK_HEIGHT 64.0f
#define MOVE_SPEED 5.0f
#define STOP_SPEED 0.1f

#define MAX_HORIZONTAL_SPEED 5.0f

#define WEAPON_RECT 64

Game::Game(MatchStateMonitor& _monitor, Queue<GameloopMessage>& queue, MapInfo& _map_info):
        weapons(),
        message_queue(queue),
        is_running(false),
        next_player_id(0),
        round_number(0),
        monitor(_monitor),
        action_handler(ducks),
        next_bullet_id(0),
        map_info(_map_info) {}


void Game::addPlayer(DuckIdentity& duck_info, const MapInfo& map_info) {
    Position initial_pos{100 + (duck_info.id * 100), 100};  // Example starting position
    try {
        int x = map_info.respawns[duck_info.id].x;
        int y = map_info.respawns[duck_info.id].y -
                20;  // el menos 20 es para evitar colision con plataforma
        initial_pos = Position{x, y};
    } catch (const std::exception& e) {
        std::cout << "not enough respawns for all players" << std::endl;
    }
    Weapon initial_weapon;
    ducks[duck_info.id] = std::make_unique<Duck>(duck_info.id, 100, 1, initial_pos, initial_weapon,
                                                 duck_info.name);
    duck_info.initial_pos_x = initial_pos.x;
    duck_info.initial_pos_y = initial_pos.y;
}


void Game::removePlayer(uint8_t player_id) {
    ducks.erase(player_id);
    if (ducks.empty()) {
        stop();
    }
}

DuckHitbox Game::getDuckHitbox(const Duck* duck) {
    DuckHitbox hitbox;
    hitbox.leftX = duck->position.x + (DUCK_WIDTH * 0.25f);
    hitbox.rightX = duck->position.x + (DUCK_WIDTH * 0.75f);
    hitbox.topY = duck->position.y + (DUCK_HEIGHT * 0.25f);
    hitbox.bottomY = duck->position.y + DUCK_HEIGHT;
    return hitbox;
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

void Game::updateBullets() {
    for (auto it = bullets_by_id.begin(); it != bullets_by_id.end();) {

        Bullet& bullet = it->second;

        bool errase = false;

        for (auto& duck_pair: ducks) {
            Duck* duck = duck_pair.second.get();
            if (bullet.duck_how_shot == duck->duck_id) {
                continue;
            }

            if (duck->position.x < bullet.x + 10 && duck->position.x + DUCK_WIDTH > bullet.x &&
                duck->position.y < bullet.y + 10 && duck->position.y + DUCK_HEIGHT > bullet.y) {

                duck->receive_damage(bullet.damage);
                errase = true;
            }
        }

        if (bullet.x < 0 || bullet.x > 1200 || bullet.y < 0 || bullet.y > 1200 || errase) {
            // it = bullets_by_id.erase(it);
            bullet.update(Bullet());
            it++;
            continue;
        } else {
            it++;
            bullet.move();
        }
    }
}

void Game::updateDucks(std::shared_ptr<std::vector<DuckState>>& duck_states) {
    for (auto& duck_pair: ducks) {
        Duck* duck = duck_pair.second.get();

        updateDuck(duck, duck_states);
    }
}

void Game::checkShoot(Duck* duck) {
    if (duck->is_shooting && duck->weapon.ammo > 0) {
        duck->weapon.shoot(duck->looking == 1, duck->position.x, DUCK_WIDTH, duck->position.y,
                           DUCK_HEIGHT, bullets_by_id, next_bullet_id, duck->duck_id);
    }
}

void Game::updateDuckVerticalPosition(Duck* duck) {

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
    bool above_platform = false;
    for (const auto& platform: map_info.platforms) {
        if (duck->position.x < platform.x + platform.width &&
            duck->position.x + DUCK_WIDTH > platform.x &&
            duck->position.y < platform.y + platform.height &&
            duck->position.y + DUCK_HEIGHT > platform.y) {
            above_platform = true;
        }
    }
    if (!above_platform) {
        duck->in_air = true;
        duck->vertical_velocity += GRAVITY;
    }

    // Limit fall speed
    if (duck->vertical_velocity > MAX_FALL_SPEED) {
        duck->vertical_velocity = MAX_FALL_SPEED;
    }

    // Apply vertical velocity
    duck->position.y += duck->vertical_velocity;
    duck->is_falling = duck->vertical_velocity > 0;
}

void Game::updateDuckHorizontalPosition(Duck* duck) {
    if (duck->is_running) {
        duck->horizontal_velocity += MOVE_SPEED;
        if (duck->horizontal_velocity > MAX_HORIZONTAL_SPEED) {
            duck->horizontal_velocity = MAX_HORIZONTAL_SPEED;
        }
        if (duck->looking == 1) {
            double actual_pos = duck->position.x;
            double new_pos = actual_pos + duck->horizontal_velocity;
            duck->position.x = new_pos;
        } else if (duck->looking == 0) {
            duck->position.x -= duck->horizontal_velocity;
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
}

void Game::updateDuckState(Duck* duck) {

    // Update vertical movement
    updateDuckVerticalPosition(duck);

    // Update horizontal position
    updateDuckHorizontalPosition(duck);
}

void Game::checkWeaponPickupCollision(Duck* duck, const std::vector<Weapon>& weapons,
                                      const DuckHitbox& hitbox) {
    for (const auto& weapon: weapons) {
        if (hitbox.leftX < weapon.pos.x + WEAPON_RECT && hitbox.rightX > weapon.pos.x &&
            hitbox.topY < weapon.pos.y + WEAPON_RECT && hitbox.bottomY > weapon.pos.y) {
            duck->pick_up_weapon(weapon);
        }
    }
}

void Game::checkPlatformsCollision(Duck* duck, const std::vector<Platform>& platforms,
                                   float previous_x, float previous_y) {
    for (const auto& platform: platforms) {
        if (checkPlatformCollision(duck->position, DUCK_WIDTH, DUCK_HEIGHT, platform)) {
            if (duck->vertical_velocity > 0 && previous_y + DUCK_HEIGHT <= platform.y) {
                // Landing on platform
                // Small adjustment to avoid a horizontal
                // collision with the platform
                duck->position.y = platform.y - DUCK_HEIGHT - 2;
                duck->vertical_velocity = 0;
                duck->in_air = false;
                duck->is_jumping = false;
                duck->is_gliding = false;
            } else if (duck->vertical_velocity < 0 && previous_y >= platform.y + platform.height) {
                // Hitting platform from below
                duck->position.y = platform.y + platform.height;
                duck->vertical_velocity = 0;
            } else {
                // Side collision
                duck->position.x = previous_x;
            }
        }
    }
}

void Game::checkCollisions(Duck* duck, const std::vector<Platform>& platforms, float previous_x,
                           float previous_y) {

    // Check platform collisions
    checkPlatformsCollision(duck, platforms, previous_x, previous_y);

    const float ground_level = 700.0f - DUCK_HEIGHT;

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
}

void Game::updateDuck(Duck* duck, std::shared_ptr<std::vector<DuckState>>& duck_states) {


    DuckHitbox hitbox = getDuckHitbox(duck);

    // Checking if a duck collides with a weapon to pick it
    // up
    checkWeaponPickupCollision(duck, weapons, hitbox);

    // Checking if it is shooting
    checkShoot(duck);
    // Store previous position for collision resolution
    float previous_y = duck->position.y;
    float previous_x = duck->position.x;

    updateDuckState(duck);

    // Check platform collisions
    checkCollisions(duck, map_info.platforms, previous_x, previous_y);

    // Update duck state
    DuckState state(duck->name, duck->duck_id, duck->life_points, duck->looking, duck->position,
                    duck->is_alive ? 1 : 0, duck->is_running ? 1 : 0, duck->is_jumping ? 1 : 0,
                    duck->is_gliding ? 1 : 0, duck->is_falling ? 1 : 0, duck->is_ducking ? 1 : 0,
                    duck->is_shooting ? 1 : 0, duck->is_sliding ? 1 : 0, duck->helmet_on ? 1 : 0,
                    duck->armor_on ? 1 : 0, duck->in_air ? 1 : 0, duck->vertical_velocity,
                    duck->horizontal_velocity, duck->weapon.getType());

    duck->update_state(state);
    duck_states->push_back(state);
}

void Game::updateGameState() {

    std::shared_ptr<std::vector<DuckState>> duck_states =
            std::make_shared<std::vector<DuckState>>();
    std::shared_ptr<std::vector<Bullet>> bullets_in_game = std::make_shared<std::vector<Bullet>>();

    // Updating the ducks
    updateDucks(duck_states);

    // Updating the bullets
    updateBullets();


    std::transform(bullets_by_id.begin(), bullets_by_id.end(), std::back_inserter(*bullets_in_game),
                   [](const auto& pair) { return pair.second; });

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
        updateGameState();  // si no pongo esto no le envia a
                            // los clientes los patos que
                            // tienen 0 de vida, muy raro,
                            // pero bueno
        sleep(1);

        startNewRound();
    }
}

void Game::startNewRound() {
    round_number++;
    std::cout << "Starting new round " << round_number << std::endl;
    next_bullet_id = 0;
    bullets_by_id.clear();


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
    // No se y no entiendo por que mi cppcheck local se pone
    // como loca de que debería usar algoritmos de la
    // librería estándar para esto aunque haga que el código
    // sea horrible, estoy hace horas tratando de que no pase
    // pero ni idea por que carajo sigue igual, otro dia se
    // verá
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
    WeaponFactory weapon_factory;
    for (auto weapon: map_info.weapons) {
        Weapon new_weapon = weapon_factory.createWeapon(weapon);
        weapons.push_back(new_weapon);
    }
    is_running = true;
    send_platforms_first_time();
    try {
        while (is_running) {
            double start_time = getCurrentTime();

            GameloopMessage msg(0, 0);
            while (message_queue.try_pop(msg)) {
                action_handler.process_player_action(msg);
            }
            updateGameState();
            checkRoundEnd();


            double end_time = getCurrentTime();
            rateController(start_time, end_time);
        }
    } catch (ClosedQueue& e) {
        stop();  // si se cierra la cola de mensajes se
                 // termina el juego, la puede cerrar el
                 // monitor_match legalmente
    } catch (const std::exception& e) {
        std::cerr << "Error occurred during game loop: " << e.what() << std::endl;
        stop();
    }
}

void Game::send_platforms_first_time() {
    std::vector<Platform> platforms = map_info.platforms;
    Snapshot snapshot;
    for (const auto& plat: platforms) {
        snapshot.addPlatform(plat);
    }
    monitor.push_to_all(std::make_shared<Snapshot>(snapshot));
}

void Game::stop() { is_running = false; }

bool Game::isRunning() const { return is_running; }
