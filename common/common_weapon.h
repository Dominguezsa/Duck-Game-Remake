#ifndef COMMON_WEAPON_H
#define COMMON_WEAPON_H

#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <utility>

#include "types/weapon_type.h"

#include "bullet.h"
#include "common_position.h"

class Weapon {
public:
    uint8_t id = 0;  // id del arma
    std::string name = "None";
    WeaponType type = NoneType;
    uint8_t ammo = 0;
    uint8_t cicles_to_reshoot = 0;  // esto seria que tan rapido es el arma disparando, osea cada
                                    // cuantos ciclos de juego se puede disparar
    uint8_t actual_cicle = 0;
    uint8_t damage = 0;
    Position pos = Position(0, 0);
    float bullet_speed = 0;
    uint8_t bullet_time = 0;
    uint8_t spread_angle = 0;


public:
    Weapon(uint8_t id, const std::string& name, uint8_t ammo, uint8_t cicles_to_reshoot,
           uint8_t damage);

    Weapon(uint8_t id, const std::string& name, uint8_t ammo, uint8_t cicles_to_reshoot,
           uint8_t damage, Position pos, WeaponType type, uint8_t bullet_speed, uint8_t bullet_time,
           uint8_t spread_angle);

    Weapon(uint8_t id, Position pos);

    WeaponType getType() const { return type; }
    Weapon() = default;

    void shoot(bool going_right, float duck_pos_x, float duck_pos_y,
               std::map<std::pair<uint32_t, uint8_t>, Bullet>& bullets_by_id,
               uint32_t& next_bullet_id, uint8_t duck_id);

    void update(const Weapon& other);

    Weapon ak47();
    Weapon grenade();
    Weapon banana();
    Weapon pewPewLaser();
    Weapon laserRifle();
    Weapon shotgun();
    Weapon sniper();
    Weapon duelingPistol();
    Weapon magnum();
    Weapon cowboyPistol();
};

#endif  // COMMON_WEAPON_H
