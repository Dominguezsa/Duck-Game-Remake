#include "common_weapon.h"

Weapon::Weapon::Weapon(uint8_t id, const std::string& name, uint8_t ammo, uint8_t cicles_to_reshoot,
                       uint8_t damage):
        id(id),
        name(name),
        ammo(ammo),
        cicles_to_reshoot(cicles_to_reshoot),
        damage(damage),
        bullet_speed(0),
        bullet_time(0) {}

Weapon::Weapon(uint8_t id, const std::string& name, uint8_t ammo, uint8_t cicles_to_reshoot,
               uint8_t damage, Position pos, WeaponType type, uint8_t bullet_speed,
               uint8_t bullet_time, uint8_t spread_angle):
        id(id),
        name(name),
        type(type),
        ammo(ammo),
        cicles_to_reshoot(cicles_to_reshoot),
        damage(damage),
        pos(pos),
        bullet_speed(bullet_speed),
        bullet_time(bullet_time),
        spread_angle(spread_angle) {}

Weapon::Weapon(uint8_t id, Position pos):
        id(id),
        name(""),
        type(WeaponType::NoneType),
        ammo(0),
        cicles_to_reshoot(0),
        damage(0),
        pos(pos),
        bullet_speed(0),
        bullet_time(0) {}

void Weapon::shoot(bool going_right, float duck_pos_x, float duck_pos_y,
                   std::map<std::pair<uint32_t, uint8_t>, Bullet>& bullets_by_id,
                   uint32_t& next_bullet_id, uint8_t duck_id) {
    if (actual_cicle == 0) {
        int angle = 0;
        if (spread_angle != 0) {
            angle += std::rand() % (spread_angle * 2) - spread_angle;
        }
        Bullet bullet(id, duck_pos_x, duck_pos_y, angle, bullet_speed, bullet_time, going_right,
                      damage, duck_id);

        bullets_by_id.insert({{next_bullet_id, bullet.id}, bullet});
        next_bullet_id++;
        ammo--;
        actual_cicle++;
    } else {
        if (actual_cicle == cicles_to_reshoot) {
            actual_cicle = 0;
        } else {
            actual_cicle++;
        }
    }
}

void Weapon::update(const Weapon& other) {
    id = other.id;
    name = other.name;
    type = other.type;
    ammo = other.ammo;
    cicles_to_reshoot = other.cicles_to_reshoot;
    actual_cicle = other.actual_cicle;
    damage = other.damage;
    pos = other.pos;
    bullet_speed = other.bullet_speed;
    bullet_time = other.bullet_time;
    spread_angle = other.spread_angle;
}

Weapon Weapon::ak47() { return Weapon(WEAPON_TYPE::AK47, "ak47", 30, 5, 10); }

Weapon Weapon::grenade() { return Weapon(WEAPON_TYPE::Granade, "grenade", 5, 10, 100); }

Weapon Weapon::banana() { return Weapon(WEAPON_TYPE::Banana, "banana", 5, 10, 0); }

Weapon Weapon::pewPewLaser() { return Weapon(WEAPON_TYPE::PewPewLaser, "pewPewLaser", 5, 10, 50); }

Weapon Weapon::laserRifle() { return Weapon(WEAPON_TYPE::LaserRifle, "laserRifle", 5, 10, 50); }

Weapon Weapon::shotgun() { return Weapon(WEAPON_TYPE::Shotgun, "shotgun", 5, 10, 50); }

Weapon Weapon::sniper() { return Weapon(WEAPON_TYPE::Sniper, "sniper", 3, 10, 100); }

Weapon Weapon::duelingPistol() {
    return Weapon(WEAPON_TYPE::DuelPistol, "duelingPistol", 5, 10, 50);
}

Weapon Weapon::magnum() { return Weapon(WEAPON_TYPE::Magnum, "magnum", 5, 10, 50); }

Weapon Weapon::cowboyPistol() {
    return Weapon(WEAPON_TYPE::CowboyPistol, "cowboyPistol", 5, 10, 50);
}
