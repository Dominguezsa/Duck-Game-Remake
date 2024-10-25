#include "duck.h"

Duck::Duck(uint8_t id, uint8_t vida, bool looking, Position pos, const Weapon& weapon):
        duck_id(id),
        life_points(vida),
        looking(looking),
        is_alive(true),
        is_running(false),
        is_jumping(false),
        is_ducking(false),
        is_shooting(false),
        helmet_on(false),
        armor_on(false),
        position(pos),
        weapon(weapon) {}

void Duck::move_to(uint8_t direccion) {
    if (direccion == 0) {
        position.x -= 1;
        looking = 0;
    } else if (direccion == 1) {
        position.x += 1;
        looking = 1;
    }
}

void Duck::jump(bool activar) {
    if (activar) {
        is_jumping = true;
    } else {
        is_jumping = false;
    }
}

void Duck::duck(bool activar) {
    if (activar) {
        is_ducking = true;
    } else {
        is_ducking = false;
    }
}

void Duck::shoot(bool activar) {
    if (activar) {
        is_shooting = true;
    } else {
        is_shooting = false;
    }
}

void Duck::receive_damage(uint8_t danio) {
    int puntos_de_vida_con_signo = life_points;
    if (puntos_de_vida_con_signo - danio <= 0) {
        life_points = 0;
        is_alive = false;
    } else {
        life_points -= danio;
    }
}

void Duck::pick_up_weapon(const Weapon& weapon) { this->weapon = weapon; }

void Duck::throw_weapon() { this->weapon = Weapon("", 0); }
