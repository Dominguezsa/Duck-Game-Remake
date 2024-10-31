#ifndef DUCK_H
#define DUCK_H

#include <cstdint>

#include "types/duck_state.h"
#include "types/weapon_type.h"

#include "common_position.h"
#include "common_weapon.h"

class Duck {
public:
    // cppcheck-suppress unusedStructMember
    uint8_t duck_id;
    // cppcheck-suppress unusedStructMember
    uint8_t life_points;
    // cppcheck-suppress unusedStructMember
    uint8_t looking;  // 0 -> izquierda, 1 -> derecha, 2 -> arriba, 3 -> abajo
    // cppcheck-suppress unusedStructMember
    bool is_alive;
    // cppcheck-suppress unusedStructMember
    bool is_running;
    // cppcheck-suppress unusedStructMember
    bool is_jumping;
    // cppcheck-suppress unusedStructMember
    bool is_ducking;
    // cppcheck-suppress unusedStructMember
    bool is_shooting;
    // cppcheck-suppress unusedStructMember
    bool helmet_on;
    // cppcheck-suppress unusedStructMember
    bool armor_on;
    // cppcheck-suppress unusedStructMember
    float vertical_velocity = 0.0f;
    // cppcheck-suppress unusedStructMember
    bool in_air = true;
    // cppcheck-suppress unusedStructMember
    Position position;
    // cppcheck-suppress unusedStructMember
    Weapon weapon;

    // ------------------- Constructores -------------------

    explicit Duck(uint8_t id, uint8_t vida, bool mira_hacia, Position pos, const Weapon& weapon);

    // Constructor sin parametros
    Duck();

    // Constructor de copia
    Duck(const Duck& other);

    // ------------------- Métodos -------------------

    // Asignación por copia
    Duck& operator=(const Duck& other);

    void move_to(uint8_t direccion);  // 0: izquierda, 1: derecha (hacia arriba seria un salto y
                                      // hacia abajo se agacha)

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
