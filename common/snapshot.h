#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <vector>

#include "bullet.h"
#include "types/duck_state.h"

struct Snapshot {
public:
    std::vector<DuckState> ducks;
    std::vector<Bullet> bullets;

    Snapshot(std::vector<DuckState>& ducks, std::vector<Bullet>& bullets)
        : ducks(ducks), bullets(bullets) {}
};

#endif  // SNAPSHOT_H