#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "../common/types/match_state.h"
#include "../common/weapons/common_weapon.h"
#include "../editor/image_type.h"

class WeaponFactory {
private:
    using WeaponCreator = std::function<Weapon(const weapons_in_map)>;

    std::unordered_map<uint8_t, WeaponCreator> weaponCreators;

public:
    WeaponFactory() {
        weaponCreators[ImageType::ak47] = [](const weapons_in_map& data) {
            return Weapon(WeaponType::AK47, "ak47", 30, 10, 25,
                          {static_cast<int>(data.x), static_cast<int>(data.y)}, WeaponType::AK47,
                          7.0f, 0.0f, 10, 1.3, 1.3);
        };

        weaponCreators[ImageType::sniper] = [](const weapons_in_map& data) {
            return Weapon(WeaponType::Sniper, "sniper", 3, 10000, 80,
                          {static_cast<int>(data.x), static_cast<int>(data.y)}, WeaponType::Sniper,
                          10.0f, 0.0f, 0, 1.3, 1.7);
        };

        weaponCreators[ImageType::shotgun] = [](const weapons_in_map& data) {
            return Weapon(WeaponType::Shotgun, "shotgun", 3, 10000, 20,
                          {static_cast<int>(data.x), static_cast<int>(data.y)}, WeaponType::Shotgun,
                          7.0f, 0.0f, 10, 1.3, 1.3);
        };

        weaponCreators[ImageType::duelingGun] = [](const weapons_in_map& data) {
            return Weapon(WeaponType::DuelPistol, "duelingPistol", 1, 10000, 50,
                          {static_cast<int>(data.x), static_cast<int>(data.y)},
                          WeaponType::DuelPistol, 6.0f, 0.0f, 10, 1.3, 1.9);
        };

        weaponCreators[ImageType::banana1] = [](const weapons_in_map& data) {
            return Weapon(WeaponType::Banana, "banana", 0, 10, 0,
                          {static_cast<int>(data.x), static_cast<int>(data.y)}, WeaponType::Banana,
                          0.0f, 0.0f, 0, 1.3, 1.3);
        };

        weaponCreators[ImageType::grenade1] = [](const weapons_in_map& data) {
            return Weapon(WeaponType::Granade, "grenade", 0, 10, 100,
                          {static_cast<int>(data.x), static_cast<int>(data.y)}, WeaponType::Granade,
                          0.0f, 0.0f, 0, 1.3, 1.3);
        };

        weaponCreators[ImageType::laserRifle] = [](const weapons_in_map& data) {
            return Weapon(WeaponType::LaserRifle, "laserRifle", 30, 9, 15,
                          {static_cast<int>(data.x), static_cast<int>(data.y)},
                          WeaponType::LaserRifle, 7.0f, 0.0f, 2, 1.3, 1.3);
        };

        weaponCreators[ImageType::PewPewLaser1] = [](const weapons_in_map& data) {
            return Weapon(WeaponType::PewPewLaser, "PewPewLaser", 5, 0, 25,
                          {static_cast<int>(data.x), static_cast<int>(data.y)},
                          WeaponType::PewPewLaser, 3.0f, 0.0f, 5, 1.3, 1.3);
        };

        weaponCreators[ImageType::cowboyGun] = [](const weapons_in_map& data) {
            return Weapon(WeaponType::CowboyPistol, "cowboyGun", 5, 1000, 15,
                          {static_cast<int>(data.x), static_cast<int>(data.y)},
                          WeaponType::CowboyPistol, 6.0f, 0.0f, 0, 1.3, 1.3);
        };

        weaponCreators[ImageType::magnum] = [](const weapons_in_map& data) {
            return Weapon(WeaponType::Magnum, "magnum", 5, 10, 70,
                          {static_cast<int>(data.x), static_cast<int>(data.y)}, WeaponType::Magnum,
                          7.0f, 0.0f, 15, 1.3, 1.3);
        };
    }

    Weapon createWeapon(const weapons_in_map& weaponData) const {
        auto it = weaponCreators.find(weaponData.id);
        if (it != weaponCreators.end()) {
            return it->second(weaponData);
        }
        return Weapon();
    }
};
