#ifndef COMMON_ARMA_H
#define COMMON_ARMA_H

#include <cstdint>
#include <string>

struct Arma {
    std::string nombre;
    uint8_t municion;

    // Constructor
    Arma(const std::string& nombre, uint8_t municion): nombre(nombre), municion(municion) {}
};

#endif  // COMMON_ARMA_H
