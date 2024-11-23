#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <vector>

#include "types/duck_state.h"
#include "common_weapon.h"
#include "bullet.h"

struct Snapshot {
public:
    std::vector<DuckState> ducks;
    std::vector<Bullet> bullets;
    std::vector<Weapon> weapons;

    Snapshot(std::vector<DuckState>& ducks, std::vector<Bullet>& bullets, std::vector<Weapon>& weapons):
            ducks(ducks), bullets(bullets), weapons(weapons) {}

    Snapshot() = default;
};

#endif  // SNAPSHOT_H
