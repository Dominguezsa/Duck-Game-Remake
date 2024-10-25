#ifndef DUCK_H
#define DUCK_H

#include <cstdint>

#include "common_weapon.h"
#include "common_position.h"

class Duck {
public:
    uint8_t duck_id;
    uint8_t life_points;
    uint8_t looking;  // 0 -> izquierda, 1 -> derecha, 2 -> arriba, 3 -> abajo
    bool is_alive;
    bool is_running;
    bool is_jumping;
    bool is_ducking;
    bool is_shooting;
    bool helmet_on;
    bool armor_on;
    Position position;
    Weapon weapon;


    explicit Duck(uint8_t id, uint8_t vida, bool mira_hacia, Position pos, const Weapon& weapon);
    void move_to(uint8_t direccion);  // 0: izquierda, 1: derecha (hacia arriba seria un salto y
                                          // hacia abajo se agacha)

    void jump(bool activar);  // recibe activar para saber si se activa o desactiva el salto (y no
                                // implementar 2 mensajes)

    void duck(bool activar);

    void shoot(bool activar);

    void receive_damage(uint8_t danio);

    void pick_up_weapon(const Weapon& weapon);

    void throw_weapon();
};

#endif  // DUCK_H
