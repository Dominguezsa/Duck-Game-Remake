#ifndef COMMON_WEAPON_H
#define COMMON_WEAPON_H

#include <cstdint>
#include <iostream>
#include <string>

#include "types/weapon_type.h"

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


public:
    Weapon(uint8_t id, const std::string& name, uint8_t ammo, uint8_t cicles_to_reshoot,
           uint8_t damage):
            id(id), name(name), ammo(ammo), cicles_to_reshoot(cicles_to_reshoot), damage(damage) {}

    Weapon(uint8_t id, const std::string& name, uint8_t ammo, uint8_t cicles_to_reshoot,
           uint8_t damage, Position pos, WeaponType type):
            id(id),
            name(name),
            type(type),
            ammo(ammo),
            cicles_to_reshoot(cicles_to_reshoot),
            damage(damage),
            pos(pos) {}

    Weapon(uint8_t id, Position pos): id(id), pos(pos) {}

    WeaponType getType() const { return type; }

    Weapon() = default;

    void update(const Weapon& other) {
        id = other.id;
        name = other.name;
        type = other.type;
        ammo = other.ammo;
        cicles_to_reshoot = other.cicles_to_reshoot;
        actual_cicle = other.actual_cicle;
        damage = other.damage;
        pos = other.pos;
    }

    Weapon ak47() {
        return Weapon(WEAPON_TYPE::AK47, "ak47", 30, 5, 10);
    }

    Weapon grenade() {
        return Weapon(WEAPON_TYPE::Granade, "grenade", 5, 10, 100);
    }

    Weapon banana() {
        return Weapon(WEAPON_TYPE::Banana, "banana", 5, 10, 0);
    }

    Weapon pewPewLaser() {
        return Weapon(WEAPON_TYPE::PewPewLaser, "pewPewLaser", 5, 10, 50);
    }

    Weapon laserRifle() {
        return Weapon(WEAPON_TYPE::LaserRifle, "laserRifle", 5, 10, 50);
    }

    Weapon shotgun() {
        return Weapon(WEAPON_TYPE::Shotgun, "shotgun", 5, 10, 50);
    }

    Weapon sniper() {
        return Weapon(WEAPON_TYPE::Sniper, "sniper", 3, 10, 100);
    }

    Weapon duelingPistol() {
        return Weapon(WEAPON_TYPE::DuelPistol, "duelingPistol", 5, 10, 50);
    }

    Weapon magnum() {
        return Weapon(WEAPON_TYPE::Magnum, "magnum", 5, 10, 50);
    }

    Weapon cowboyPistol() {
        return Weapon(WEAPON_TYPE::CowboyPistol, "cowboyPistol", 5, 10, 50);
    }
};

#endif  // COMMON_WEAPON_H
