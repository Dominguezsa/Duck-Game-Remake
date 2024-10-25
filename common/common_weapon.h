#ifndef COMMON_WEAPON_H
#define COMMON_WEAPON_H

#include <cstdint>
#include <string>

struct Weapon {
    std::string name;
    uint8_t ammo;

    // Constructor
    Weapon(const std::string& name, uint8_t ammo): name(name), ammo(ammo) {}
};

#endif  // COMMON_WEAPON_H
