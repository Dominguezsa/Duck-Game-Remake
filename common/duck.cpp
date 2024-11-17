#include "duck.h"

#include <iostream>

// ------------------- Constructores -------------------

#define JUMP_SPEED 15.0f

Duck::Duck(uint8_t id, uint8_t vida, bool looking, Position pos, const Weapon& weapon):
        duck_id(id),
        life_points(vida),
        looking(looking),
        is_alive(true),
        is_running(false),
        is_jumping(false),
        is_gliding(false),
        is_falling(false),
        is_ducking(false),
        is_shooting(false),
        helmet_on(false),
        armor_on(false),
        position(pos),
        weapon(weapon) {}

Duck::Duck():
        duck_id(0),
        life_points(0),
        looking(0),
        is_alive(false),
        is_running(false),
        is_jumping(false),
        is_gliding(false),
        is_falling(false),
        is_ducking(false),
        is_shooting(false),
        helmet_on(false),
        armor_on(false),
        position({0, 0}),
        weapon(Weapon(AK47, "ak47", 0, 15, 20)) {}

Duck::Duck(const Duck& other):
        duck_id(other.duck_id),
        life_points(other.life_points),
        looking(other.looking),
        is_alive(other.is_alive),
        is_running(other.is_running),
        is_jumping(other.is_jumping),
        is_gliding(other.is_gliding),
        is_falling(other.is_falling),
        is_ducking(other.is_ducking),
        is_shooting(other.is_shooting),
        helmet_on(other.helmet_on),
        armor_on(other.armor_on),
        position(other.position),
        weapon(other.weapon) {}

// ------------------- Métodos -------------------

Duck& Duck::operator=(const Duck& other) {
    if (this != &other) {
        duck_id = other.duck_id;
        life_points = other.life_points;
        looking = other.looking;
        is_alive = other.is_alive;
        is_running = other.is_running;
        is_jumping = other.is_jumping;
        is_gliding = other.is_gliding;
        is_falling = other.is_falling;
        is_ducking = other.is_ducking;
        is_shooting = other.is_shooting;
        helmet_on = other.helmet_on;
        armor_on = other.armor_on;
        vertical_velocity = 0.0f;
        position = other.position;
        weapon = other.weapon;
    }
    return *this;
}

void Duck::move_to(uint8_t direccion) {

    if (direccion > 1) {
        return;
    }

    is_running = true;

    looking = direccion;

    // if (direccion == 0) {
    //     looking = 0;
    // } else if (direccion == 1) {
    //     looking = 1;
    // }
}

void Duck::look_to(uint8_t direccion) {
    if (direccion > 3) {
        return;
    }

    looking = direccion;
}

void Duck::stop_running() { is_running = false; }

void Duck::jump(bool activar) {
    // if true, spacebar down; if false, spacebar up
    if (activar && is_jumping) {
        is_gliding = true;
        vertical_velocity = 0.0f;
        return;
    }
    if (activar) {
        is_jumping = true;
        vertical_velocity = -JUMP_SPEED;
        in_air = true;
    } else {
        is_gliding = false;
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

void Duck::throw_weapon() { this->weapon = Weapon(AK47, "", 0, 15, 20); }

void Duck::get_state(DuckState& state) {
    state.duck_id = static_cast<uint8_t>(duck_id);
    state.life_points = life_points;
    state.looking = looking;
    state.position = position;
    state.is_alive = static_cast<uint8_t>(is_alive);
    state.is_running = static_cast<uint8_t>(is_running);
    state.is_jumping = static_cast<uint8_t>(is_jumping);
    state.is_ducking = static_cast<uint8_t>(is_ducking);
    state.is_shooting = static_cast<uint8_t>(is_shooting);
    state.helmet_on = static_cast<uint8_t>(helmet_on);
    state.armor_on = static_cast<uint8_t>(armor_on);
    state.is_gliding = static_cast<uint8_t>(is_gliding);
    state.is_falling = static_cast<uint8_t>(is_falling);
    state.in_air = static_cast<uint8_t>(in_air);
    state.vertical_velocity = vertical_velocity;
    // TO-DO: Ajustar el uso del tipo de weapon a la
    //        clase correspondiente.
    state.weapon = WeaponType::NoneType;
}

void Duck::update_state(const DuckState& state) {
    duck_id = state.duck_id;
    life_points = state.life_points;
    looking = state.looking;
    position = state.position;
    is_alive = static_cast<bool>(state.is_alive);
    is_running = static_cast<bool>(state.is_running);
    is_jumping = static_cast<bool>(state.is_jumping);
    is_gliding = static_cast<bool>(state.is_gliding);
    is_falling = static_cast<bool>(state.is_falling);
    in_air = static_cast<bool>(state.in_air);
    vertical_velocity = state.vertical_velocity;
    is_ducking = static_cast<bool>(state.is_ducking);
    is_shooting = static_cast<bool>(state.is_shooting);
    helmet_on = static_cast<bool>(state.helmet_on);
    armor_on = static_cast<bool>(state.armor_on);
    // TO-DO: Ajustar el uso del tipo de weapon.
    weapon = Weapon(AK47, "ak47", 0, 15, 20);
}
