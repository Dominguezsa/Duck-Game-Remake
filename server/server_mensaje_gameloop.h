#ifndef SERVER_MENSAJE_GAMELOOP_H
#define SERVER_MENSAJE_GAMELOOP_H

#include <cstdint>
#include <string>

struct MensajeGameloop {
    uint8_t jugador_id;
    uint8_t accion;

    // Constructor
    MensajeGameloop(uint8_t jugador_id, uint8_t accion): jugador_id(jugador_id), accion(accion) {}
};

#endif  // SERVER_MENSAJE_GAMELOOP_H
