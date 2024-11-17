#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <vector>

#include "bullet.h"
#include "duck_state.h"

struct Snapshot {
    std::vector<DuckState> ducks;
    std::vector<Bullet> bullets;
};

#endif  // SNAPSHOT_H