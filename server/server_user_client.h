#ifndef SERVER_USER_CLIENT_H
#define SERVER_USER_CLIENT_H

#include <cstdint>

#include "../common/common_socket.h"
#include "../common/common_protocol.h"
#include "server_receiver.h"
#include "server_sender.h"

class UserClient {
    private:    
        uint8_t id;
        Socket skt;
        ServerProtocol protocol;
        ReceiverThread receiver;
        SenderThread sender;
    
    public:
        UserClient(Queue<GameloopMessage> &gameloop_queue, Socket &&skt, uint8_t id);
        
        bool is_alive();

        uint8_t get_id() const;

        Queue<DuckState>* get_queue();

        void end_communication();

        ~UserClient() = default;
};

#endif // SERVER_USER_CLIENT_H