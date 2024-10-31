#include "common_protocol.h"

#include <iostream>

#include <sys/socket.h>

Protocol::Protocol(Socket& skt): skt(skt) {}

void Protocol::end_communication() {
    this->skt.shutdown(SHUT_RDWR);
    this->skt.close();
}

void Protocol::recv_uint_32(uint32_t& received) {
    bool was_closed = false;
    this->skt.recvall(&received, sizeof(uint32_t), &was_closed);
    if (was_closed) {
        throw SocketWasCLosedException(errno, "Socket was closed while receiving a uint32_t.\n");
    }
    received = ntohl(received);
}

void Protocol::recv_uint_16(uint16_t& received) {
    bool was_closed = false;
    this->skt.recvall(&received, sizeof(uint16_t), &was_closed);
    if (was_closed) {
        throw SocketWasCLosedException(errno, "Socket was closed while receiving a uint16_t.\n");
    }
    received = ntohs(received);
}

void Protocol::recv_uint_8(uint8_t& received) {
    bool was_closed = false;
    this->skt.recvall(&received, sizeof(uint8_t), &was_closed);
    if (was_closed) {
        throw SocketWasCLosedException(errno, "Socket was closed while receiving a uint8_t.\n");
    }
    // std::cout << "I received: " << std::hex << +received << std::endl;
}

void Protocol::recv_string(std::string& received) {
    uint16_t len_msg = 0;
    recv_uint_16(len_msg);
    received.resize(len_msg);

    bool was_closed = false;
    this->skt.recvall(received.data(), len_msg, &was_closed);
    if (was_closed) {
        throw SocketWasCLosedException(errno, "Socket was closed while receiving a string.\n");
    }
}

void Protocol::send_data(const void* buf, size_t buf_sz) {
    bool was_closed = false;
    this->skt.sendall(buf, buf_sz, &was_closed);
    if (was_closed) {
        std::string err_msg = "Socket was closed while sending a data buffer of size ";
        err_msg += std::to_string(buf_sz) + "\n";
        throw SocketWasCLosedException(errno, err_msg.c_str());
    }
}

void Protocol::send_string(const std::string& str_msg) {
    uint16_t len_msg = static_cast<uint16_t>(str_msg.size());
    len_msg = htons(len_msg);
    send_data(&len_msg, sizeof(uint16_t));
    send_data(str_msg.data(), str_msg.size());
}

Protocol::~Protocol() {}
