#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <cstdint>
#include <string>
#include <vector>

#include <arpa/inet.h>
#include <errno.h>

#include "common_liberror.h"
#include "common_socket.h"
#include "socket_was_closed_exception.h"

class Protocol {
protected:
    Socket& skt;

    /* Pos: Recepcion de mensaje enviado por el servidor mediante el socket
            con el que se establecio la conexion.
    */
    void recv_string(std::string& received);

    /* Pos: Se leen dos bytes mediante el socket que nos comunica con
            el servidor, los cuales representan el largo del mensaje
            que se recibira a continuacion.
    */

    void recv_float(float& received);

    void recv_uint_32(uint32_t& received);

    void recv_uint_16(uint16_t& received);

    void recv_uint_8(uint8_t& received);

    void send_data(const void* buf, size_t buf_sz);

    void send_string(const std::string& str_msg);

public:
    explicit Protocol(Socket& skt);

    virtual ~Protocol() = 0;

    // --- Impedimos la creacion de copias de la clase ---
    Protocol(const Protocol&) = delete;

    Protocol& operator=(const Protocol&) = delete;
    // ---------------------------------------------------

    // Pos: Cerramos el fd del socket.
    void end_communication();

    virtual void read_msg(void* msg) = 0;

    virtual void send_msg(void* msg) = 0;
};

#endif  // PROTOCOL
