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

void Duck::get_state(DuckState& state) {
    state.duck_id = duck_id;
    state.position = position;
    state.is_alive = is_alive;
    state.is_running = is_running;
    state.is_jumping = is_jumping;
    state.is_ducking = is_ducking;
    state.is_shooting = is_shooting;
    state.helmet_on = helmet_on;
    state.armor_on = armor_on;
    // TO-DO: Ajustar el uso del tipo de weapon a la
    //        clase correspondiente.
    state.weapon = WeaponType::NoneType;
}

void Duck::update_state(const DuckState& state) {
    duck_id = state.duck_id;
    position = state.position;
    is_alive = state.is_alive;
    is_running = state.is_running;
    is_jumping = state.is_jumping;
    is_ducking = state.is_ducking;
    is_shooting = state.is_shooting;
    helmet_on = state.helmet_on;
    armor_on = state.armor_on;
    // TO-DO: Ajustar el uso del tipo de weapon.
    // weapon = Weapon("", 0);
}
