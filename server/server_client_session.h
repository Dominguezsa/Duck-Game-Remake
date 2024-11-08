#ifndef SERVER_CLIENT_SESSION
#define SERVER_CLIENT_SESSION

#include "../common/common_thread.h"
#include "../common/common_socket.h"
#include "server_protocol.h"
#include "server_sender.h"
#include "server_matches_monitor.h"

class ClientSession : public Thread {
    private:
        uint8_t id;
        Socket skt;
        ServerProtocol protocol; // Used to receive messages only.
        SenderThread* sender;
        MatchesMonitor& matches_monitor;

    public:
        ClientSession(Socket _skt, uint8_t _id, MatchesMonitor& monitor);

        uint8_t get_id() const;

        // Pos: Ends the communication between the server and the client.
        void end_communication();

        void run() override;

        void receive_messages();
};

#endif // SERVER_CLIENT_SESSION
