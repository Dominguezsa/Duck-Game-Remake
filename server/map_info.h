#ifndef MAP_INFO_H
#define MAP_INFO_H

#include <vector>

#include "../common/types/match_state.h"
struct MapInfo {
public:
    std::vector<Platform> platforms;
    std::vector<Respawn> respawns;
    std::vector<weapons_in_map> weapons;

    MapInfo(const std::vector<Platform>& platforms, const std::vector<Respawn>& respawns,
            const std::vector<weapons_in_map>& weapons):
            platforms(platforms), respawns(respawns), weapons(weapons) {}

    MapInfo() {}

    void addPlatform(const Platform& platform) { platforms.push_back(platform); }

    void addRespawn(const Respawn& respawn) { respawns.push_back(respawn); }

    void addWeapon(const weapons_in_map& weapon) { weapons.push_back(weapon); }
};

#endif  // MAP_INFO_H
