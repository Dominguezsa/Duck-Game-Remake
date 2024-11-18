#ifndef DUCK_H
#define DUCK_H

#include <cstdint>
#include <string>

#include "types/duck_state.h"
#include "types/weapon_type.h"

#include "common_position.h"
#include "common_weapon.h"

class Duck {
public:
    std::string name;
    uint8_t duck_id;
    uint8_t life_points;
    uint8_t looking;  // 0 -> izquierda, 1 -> derecha, 2 -> arriba, 3 -> abajo
    bool is_alive;
    bool is_running;
    bool is_jumping;
    bool is_gliding;
    bool is_falling = true;
    bool is_ducking;
    bool is_shooting;
    bool is_sliding;
    bool helmet_on;
    bool armor_on;
    float vertical_velocity = 0.0f;
    float horizontal_velocity = 0.0f;
    bool in_air = true;
    Position position;
    Weapon weapon;

    // ------------------- Constructores -------------------

    explicit Duck(uint8_t id, uint8_t vida, bool mira_hacia, Position pos, const Weapon& weapon,
                  const std::string& name);

    // Constructor sin parametros
    Duck();

    // Constructor de copia
    Duck(const Duck& other);

    // ------------------- Métodos -------------------

    // Asignación por copia
    Duck& operator=(const Duck& other);

    void move_to(uint8_t direccion);  // 0: izquierda, 1: derecha (hacia arriba seria un salto y
                                      // hacia abajo se agacha)

    void stop_running();

    void look_to(uint8_t direccion);  // 0: izquierda, 1: derecha, 2: arriba, 3: abajo

    void jump(bool activar);  // recibe activar para saber si se activa o desactiva el salto (y no
                              // implementar 2 mensajes)

    void duck(bool activar);

    void shoot(bool activar);

    void receive_damage(uint8_t danio);

    void pick_up_weapon(const Weapon& weapon);

    void throw_weapon();

    void get_state(DuckState& state);

    void update_state(const DuckState& state);
};

#endif  // DUCK_H
