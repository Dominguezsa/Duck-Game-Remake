#ifndef MATCH_STATE_H
#define MATCH_STATE_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

struct Platform {
    float x;
    float y;
    float width;
    float height;
    uint8_t id;
};

struct Respawn {
    float x;
    float y;
};

struct weapons_in_map {
    float x;
    float y;
    uint8_t id;
};

struct DuckIdentity {
    std::string name;
    uint8_t id;
    float initial_pos_x;
    float initial_pos_y;
};

struct MatchInitialState {
    std::shared_ptr<std::vector<DuckIdentity>> duck_identities;
};

#endif  // MATCH_STATE_H
