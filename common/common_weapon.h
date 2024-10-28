#ifndef COMMON_WEAPON_H
#define COMMON_WEAPON_H

#include <string>
#include <cstdint>
#include "types/weapon_type.h"



class Weapon {
private:
    std::string name;
    WeaponType type;
    uint8_t ammo;

public:

    Weapon(const std::string& name, uint8_t ammo)
    : name(name), ammo(ammo) {}
    
    WeaponType getType() const {
        return type;
    }

    Weapon(WeaponType _type, uint8_t _ammo)
        : type(_type), ammo(_ammo) {}

   
};

#endif // COMMON_WEAPON_H
