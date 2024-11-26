#include "duck.h"

#include <iostream>

#include "types/weapon_type.h"

enum Directions : const uint8_t { LEFT, RIGHT, UP, DOWN };


// ------------------- Constructores -------------------

#define JUMP_SPEED 15.0f

Duck::Duck(uint8_t id, uint8_t vida, bool looking, Position pos, const Weapon& weapon,
           const std::string& name = ""):
        name(name),
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
        is_sliding(false),
        helmet_on(false),
        armor_on(false),
        position(pos),
        weapon(weapon) {}

Duck::Duck():
        duck_id(0),
        life_points(0),
        looking(LEFT),
        is_alive(false),
        is_running(false),
        is_jumping(false),
        is_gliding(false),
        is_falling(false),
        is_ducking(false),
        is_shooting(false),
        is_sliding(false),
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
        is_sliding(other.is_sliding),
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
        is_sliding = other.is_sliding;
        helmet_on = other.helmet_on;
        armor_on = other.armor_on;
        vertical_velocity = 0.0f;
        horizontal_velocity = 0.0f;
        position = other.position;
        weapon = other.weapon;
    }
    return *this;
}

void Duck::move_to(uint8_t direccion) {

    if (direccion > RIGHT) {
        return;
    }
    looking = direccion;

    if (is_ducking || is_sliding) {
        return;
    }

    is_running = true;


    // if (direccion == LEFT) {
    //     looking = LEFT;
    // } else if (direccion == RIGHT) {
    //     looking = RIGHT;
    // }
}

void Duck::look_to(uint8_t direccion) {
    if (direccion > 3) {
        return;
    }

    if (direccion != DOWN) {
        is_ducking = false;
        is_sliding = false;
    }

    // if (direccion == 2) {
    //     is_sliding = false;
    //     // return;
    // }

    looking = direccion;
}

void Duck::stop_running() { is_running = false; }

void Duck::jump(bool activar) {
    // if true, spacebar down; if false, spacebar up

    if (is_sliding || is_ducking) {
        return;
    }

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

    // if (in_air) {
    //     return;
    // }

    if (is_running) {
        is_sliding = true;
        is_running = false;
    }

    if (activar) {
        if (is_sliding) {
            return;
        }
        is_ducking = true;
    } else {

        is_running = is_sliding;

        is_sliding = false;
        is_ducking = false;
        // is_running = true;
    }
}

void Duck::shoot(bool activar) {
    if (activar) {
        if (weapon.ammo == 0 || this->weapon.getType() == NoneType) {
            return;
        }
        is_shooting = true;
        this->weapon.actual_cicle = 0;
        // std::cout << "Duck is shooting" << std::endl;
    } else {
        is_shooting = false;
        // std::cout << "Duck stopped shooting" << std::endl;
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

void Duck::pick_up_weapon(const Weapon& weapon) { this->weapon.update(weapon); }

void Duck::throw_weapon() { weapon = Weapon(); }

void Duck::get_state(DuckState& state) {
    state.name = name;
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
    state.horizontal_velocity = horizontal_velocity;
    // TO-DO: Ajustar el uso del tipo de weapon a la
    //        clase correspondiente.
    state.weapon = weapon.getType();
    state.is_sliding = static_cast<uint8_t>(is_sliding);
}

void Duck::update_state(const DuckState& state) {
    name = state.name;
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
    horizontal_velocity = state.horizontal_velocity;
    is_ducking = static_cast<bool>(state.is_ducking);
    is_shooting = static_cast<bool>(state.is_shooting);
    helmet_on = static_cast<bool>(state.helmet_on);
    armor_on = static_cast<bool>(state.armor_on);
    // TO-DO: Ajustar el uso del tipo de weapon.
    // weapon = Weapon(AK47, "ak47", 0, 15, 20);
    // weapon = Weapon(NoneType, "None", 0, 0, 0);
    is_sliding = static_cast<bool>(state.is_sliding);
}
