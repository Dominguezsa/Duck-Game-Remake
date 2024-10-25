#ifndef COMMON_POSITION_H
#define COMMON_POSITION_H

#include <cstdint>

struct Position {
    float x;
    float y;

    // Constructor
    Position(int x, int y): x(x), y(y) {}
};

#endif  // COMMON_POSITION_H
