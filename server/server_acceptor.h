#ifndef SERVER_ACCEPTOR_H
#define SERVER_ACCEPTOR_H

#include "../common/common_socket.h"
#include "../common/common_thread.h"

#include "server_match.h"
#include "server_user_client.h"

class AcceptorThread : public Thread {
    private:
        Socket acceptor_skt;
        std::list<UserClient*> clients;
        std::list<Match*> matches; // Podria ser un map con un id
                                  // que las identifique (considerarlo).
        uint8_t connection_count;
    public:
        explicit AcceptorThread(const char *servname);
        void stop() override;
        void run() override;
        void accept_connection();
        void check_unused_resources();
        void free_client_resources();
};

#endif // SERVER_ACCEPTOR_H