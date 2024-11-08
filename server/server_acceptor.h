#ifndef SERVER_ACCEPTOR_H
#define SERVER_ACCEPTOR_H

#include <list>
#include <string>
#include <utility>
#include <syslog.h>

#include "../common/common_socket.h"
#include "../common/common_thread.h"

#include "server_match.h"
#include "server_client_session.h"
//#include "server_user_client.h"

class AcceptorThread: public Thread {
private:
    Socket acceptor_skt;
    std::list<ClientSession*> clients;
    MatchesMonitor matches;
    uint8_t connection_count;

public:
    explicit AcceptorThread(const std::string& port);
    void stop() override;
    void run() override;
    void accept_connection();
    void check_unused_resources();
    void free_all_resources();
};

#endif  // SERVER_ACCEPTOR_H
