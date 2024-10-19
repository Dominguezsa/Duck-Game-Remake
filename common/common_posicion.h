#ifndef COMMON_POSICION_H
#define COMMON_POSICION_H

#include <cstdint>

struct Posicion {
    float x;
    float y;

    // Constructor
    Posicion(int x, int y): x(x), y(y) {}
};

#endif  // COMMON_POSICION_H
