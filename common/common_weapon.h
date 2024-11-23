#ifndef COMMON_WEAPON_H
#define COMMON_WEAPON_H

#include <cstdint>
#include <string>

#include "types/weapon_type.h"

#include "common_position.h"

class Weapon {
public:
    uint8_t id = 0; // id del arma 
    std::string name = "None";
    WeaponType type = NoneType;
    uint8_t ammo = 0;
    uint8_t cicles_to_reshoot = 0;  // esto seria que tan rapido es el arma disparando, osea cada
                                    // cuantos ciclos de juego se puede disparar
    uint8_t actual_cicle = 0;
    uint8_t damage = 0;
    Position pos;


public:
    Weapon(uint8_t id, const std::string& name, uint8_t ammo, uint8_t cicles_to_reshoot,
           uint8_t damage):
            id(id), name(name), ammo(ammo), cicles_to_reshoot(cicles_to_reshoot), damage(damage) {}

    Weapon(uint8_t id, const std::string& name, uint8_t ammo, uint8_t cicles_to_reshoot,
           uint8_t damage, Position pos):
            id(id),
            name(name),
            ammo(ammo),
            cicles_to_reshoot(cicles_to_reshoot),
            damage(damage),
            pos(pos) {}

    Weapon(uint8_t id, Position pos):
            id(id),
            pos(pos) {}

    WeaponType getType() const { return type; }

    Weapon() = default;
};

#endif  // COMMON_WEAPON_H
