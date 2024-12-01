#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <vector>

#include "./weapons/common_weapon.h"
#include "types/duck_state.h"
#include "types/match_state.h"

#include "bullet.h"

struct Snapshot {
public:
    std::vector<DuckState> ducks;
    std::vector<Bullet> bullets;
    std::vector<Weapon> weapons;
    std::vector<Platform> platforms;
    bool first_message = false;

    Snapshot(std::vector<DuckState>& ducks, std::vector<Bullet>& bullets,
             std::vector<Weapon>& weapons):
            ducks(ducks), bullets(bullets), weapons(weapons) {}

    Snapshot() = default;

    void addPlatform(Platform platform) {
        first_message = true;
        this->platforms.push_back(platform);
    }

    void updateSnapshot(const std::vector<DuckState>& ducks, const std::vector<Bullet>& bullets,
                        const std::vector<Weapon>& weapons) {
        for (int i = 0; i < (int)ducks.size(); i++) {
            this->ducks[i].update(ducks[i]);
        }
        // se necesita estos ifs porque cambian de tambaño y en tal caso crea las armas
        //  por default y las actualiza


        while (this->bullets.size() < bullets.size()) {
            this->bullets.push_back(Bullet());
        }
        for (int i = 0; i < (int)bullets.size(); i++) {
            this->bullets[i].update(bullets[i]);
        }
        while (this->weapons.size() < weapons.size()) {
            this->weapons.push_back(Weapon());
        }
        for (int i = 0; i < (int)weapons.size(); i++) {
            this->weapons[i].update(weapons[i]);
        }
    }
};

#endif  // SNAPSHOT_H
