#include "server_protocolo.h"

#include <stdexcept>

#include <netinet/in.h>

ServerProtocolo::ServerProtocolo(Socket& socket, uint8_t id): socket(socket), jugador_id(id) {}

MensajeGameloop ServerProtocolo::recibirMensaje() {
    bool socketCerrado = false;
    uint8_t accion;
    socket.recvall(&accion, sizeof(accion), &socketCerrado);
    if (socketCerrado) {
        throw std::logic_error("Error al recibir mensaje: socket cerrado");
    }
    accion = ntohs(accion);
    return MensajeGameloop(jugador_id, accion);
}

void ServerProtocolo::apagarSocket() {
    socket.shutdown(2);
    socket.close();
}