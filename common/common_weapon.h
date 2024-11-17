#ifndef COMMON_WEAPON_H
#define COMMON_WEAPON_H

#include <cstdint>
#include <string>

#include "types/weapon_type.h"


class Weapon {
public:
    std::string name;
    WeaponType type = NoneType;
    uint8_t ammo;
    uint8_t cicles_to_reshoot; //esto seria que tan rapido es el arma disparando, osea cada cuantos ciclos de juego se puede disparar
    uint8_t actual_cicle = 0;
    uint8_t damage;
    

public:
    Weapon(const std::string& name, uint8_t ammo, uint8_t cicles_to_reshoot, uint8_t damage)
        : name(name), ammo(ammo), cicles_to_reshoot(cicles_to_reshoot), damage(damage) {}

    WeaponType getType() const { return type; }

    Weapon(WeaponType _type, uint8_t _ammo): type(_type), ammo(_ammo) {}
};

#endif  // COMMON_WEAPON_H
