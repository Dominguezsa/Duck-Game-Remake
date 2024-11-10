#ifndef MATCH_STATE_H
#define MATCH_STATE_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

struct DuckIdentity {
    std::string name;
    // cppcheck-suppress unusedStructMember
    uint8_t id;
    // cppcheck-suppress unusedStructMember
    char color;
    // cppcheck-suppress unusedStructMember
    float initial_pos_x;
    // cppcheck-suppress unusedStructMember
    float initial_pos_y;
};

struct MatchInitialState {
    std::shared_ptr<std::vector<DuckIdentity>> duck_identities;

    // En caso de mantenerse, proximamente mas info inicial
    // de la partida, como ubicacion de plataformas, etc.
};

#endif  // MATCH_STATE_H
