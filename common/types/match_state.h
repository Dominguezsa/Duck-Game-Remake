#ifndef MATCH_STATE_H
#define MATCH_STATE_H

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

struct DuckIdentity {
    std::string name;
    uint8_t id;
    char color;
    float initial_pos_x;
    float initial_pos_y;
};

struct MatchInitialState {
    std::shared_ptr<std::vector<DuckIdentity>> duck_identities;

    // En caso de mantenerse, proximamente mas info inicial
    // de la partida, como ubicacion de plataformas, etc.
};

#endif // MATCH_STATE_H
