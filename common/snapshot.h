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
        this->bullets = bullets;
        this->weapons = weapons;
    }
};

#endif  // SNAPSHOT_H
