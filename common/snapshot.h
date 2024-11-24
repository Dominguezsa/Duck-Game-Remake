#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <vector>

#include "types/duck_state.h"

#include "bullet.h"
#include "common_weapon.h"

struct Snapshot {
public:
    std::vector<DuckState> ducks;
    std::vector<Bullet> bullets;
    std::vector<Weapon> weapons;

    Snapshot(std::vector<DuckState>& ducks, std::vector<Bullet>& bullets,
             std::vector<Weapon>& weapons):
            ducks(ducks), bullets(bullets), weapons(weapons) {}

    Snapshot() = default;

    void updateSnapshot(std::vector<DuckState>& ducks, std::vector<Bullet>& bullets,
                        std::vector<Weapon>& weapons) {
        for (int i = 0; i < (int)ducks.size(); i++) {
            this->ducks[i].update(ducks[i]);
        }
        for (int i = 0; i < (int)bullets.size(); i++) {
            this->bullets[i].update(bullets[i]);
        }
        for (int i = 0; i < (int)weapons.size(); i++) {
            this->weapons[i].update(weapons[i]);
        }
    }
};

#endif  // SNAPSHOT_H
