#ifndef SERVER_CLIENT_SESSION
#define SERVER_CLIENT_SESSION

#include "../common/common_socket.h"
#include "../common/common_thread.h"

#include "server_matches_monitor.h"
#include "server_protocol.h"
#include "server_sender.h"
#include "types/client_identity.h"

/*
    This thread listens for and processes messages from clients, which
    are classified into two types: (1) lobby messages and (2) game messages.

    (1) Lobby messages:
        
        Upon receiving lobby-related messages, it handles the following tasks:
        
        - Adds the client to an existing 'Match'.
        - Allows the client to create a new 'Match'.
        - Terminates communication with the client when required.

    (2) Game messages:

        When receiving game-related messages, it forwards them to the 'Match'
        the client is currently connected to, ensuring proper communication
        within the context of the game.
    */

#define CLIENT_QUEUE_SIZE 100 

class ClientSession: public Thread {
private:
    Identity identity;
    Socket skt;
    ServerProtocol protocol;  // Used to receive messages only.
    // SenderThread* sender;
    MatchesMonitor& matches_monitor;
    Queue<std::shared_ptr<std::vector<DuckState>>> client_queue;

public:
    ClientSession(Socket _skt, uint8_t _id, MatchesMonitor& monitor);

    uint8_t get_id() const;

    // Pos: Ends the communication between the server and the client.
    void end_communication();

    void run() override;

    void run_lobby_loop();

    void run_game_sender_loop();

    // Pos: Returns true if the action was executed successfully.
    bool exec_lobby_action(char action);
};

#endif  // SERVER_CLIENT_SESSION
