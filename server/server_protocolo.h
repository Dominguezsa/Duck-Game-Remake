#ifndef SERVER_PROTOCOLO_H
#define SERVER_PROTOCOL_H

#include <cstdint>
#include <list>
#include <vector>

#include "../common/common_socket.h"

#include "server_mensaje_gameloop.h"
#include "../common/duck.h"

class ServerProtocolo {
public:
    Socket& socket;
    uint8_t jugador_id;

    explicit ServerProtocolo(Socket& socket, uint8_t id);

    MensajeGameloop recibirMensaje();

    // std::vector<char> obtenerVectorDeCaracteresParaEnviarAlSocket(std::list<Duck> mensaje);

    void apagarSocket();
};

#endif  // SERVER_PROTOCOLO_H
