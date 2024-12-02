#include <unordered_map>
#include <functional>
#include <string>
#include <vector>

#include "../editor/image_type.h"
#include "../common/types/match_state.h"

#include "../common/weapons/common_weapon.h"

class WeaponFactory {
private:
    using WeaponCreator = std::function<Weapon(const weapons_in_map)>;

    std::unordered_map<uint8_t, WeaponCreator> weaponCreators;

public:
    WeaponFactory() {
        weaponCreators[ImageType::ak47] = [](const weapons_in_map& data) {
            return Weapon(WeaponType::AK47, "ak47", 30, 10, 10,
                          {static_cast<int>(data.x), static_cast<int>(data.y)},
                          WeaponType::AK47, 4.0f, 0.0f, 0, 1.3, 1.3);
        };

        weaponCreators[ImageType::sniper] = [](const weapons_in_map& data) {
            return Weapon(WeaponType::Sniper, "sniper", 3, 10, 100,
                          {static_cast<int>(data.x), static_cast<int>(data.y)},
                          WeaponType::Sniper, 10.0f, 0.0f, 0, 1.3, 1.3);
        };

        weaponCreators[ImageType::shotgun] = [](const weapons_in_map& data) {
            return Weapon(WeaponType::Shotgun, "shotgun", 5, 10, 50,
                          {static_cast<int>(data.x), static_cast<int>(data.y)},
                          WeaponType::Shotgun, 6.0f, 0.0f, 10, 1.3, 1.3);
        };

        weaponCreators[ImageType::duelingGun] = [](const weapons_in_map& data) {
            return Weapon(WeaponType::DuelPistol, "duelingPistol", 5, 10, 50,
                          {static_cast<int>(data.x), static_cast<int>(data.y)},
                          WeaponType::DuelPistol, 6.0f, 0.0f, 10, 1.3, 1.3);
        };

        weaponCreators[ImageType::banana1] = [](const weapons_in_map& data) {
            return Weapon(WeaponType::Banana, "banana", 5, 10, 0,
                          {static_cast<int>(data.x), static_cast<int>(data.y)},
                          WeaponType::Banana, 0.0f, 0.0f, 0, 1.3, 1.3);
        };

        weaponCreators[ImageType::grenade1] = [](const weapons_in_map& data) {
            return Weapon(WeaponType::Granade, "grenade", 5, 10, 100,
                          {static_cast<int>(data.x), static_cast<int>(data.y)},
                          WeaponType::Granade, 0.0f, 0.0f, 0, 1.3, 1.3);
        };

        weaponCreators[ImageType::laserRifle] = [](const weapons_in_map& data) {
            return Weapon(WeaponType::LaserRifle, "laserRifle", 5, 10, 100,
                          {static_cast<int>(data.x), static_cast<int>(data.y)},
                          WeaponType::LaserRifle, 0.0f, 0.0f, 0, 1.3, 1.3);
        };

        weaponCreators[ImageType::PewPewLaser1] = [](const weapons_in_map& data) {
            return Weapon(WeaponType::PewPewLaser, "PewPewLaser", 5, 10, 100,
                          {static_cast<int>(data.x), static_cast<int>(data.y)},
                          WeaponType::PewPewLaser, 0.0f, 0.0f, 0, 1.3, 1.3);
        };

        weaponCreators[ImageType::cowboyGun] = [](const weapons_in_map& data) {
            return Weapon(WeaponType::CowboyPistol, "cowboyGun", 5, 10, 100,
                          {static_cast<int>(data.x), static_cast<int>(data.y)},
                          WeaponType::CowboyPistol, 0.0f, 0.0f, 0, 1.3, 1.3);
        };

        weaponCreators[ImageType::magnum] = [](const weapons_in_map& data) {
            return Weapon(WeaponType::Magnum, "magnum", 5, 10, 100,
                          {static_cast<int>(data.x), static_cast<int>(data.y)},
                          WeaponType::Magnum, 0.0f, 0.0f, 0, 1.3, 1.3);
        };
    }

    Weapon createWeapon(const weapons_in_map& weaponData) const {
        auto it = weaponCreators.find(weaponData.id);
        if (it != weaponCreators.end()) {
            return it->second(weaponData);
        }
        throw std::runtime_error("Weapon type not found in factory");
    }
};
