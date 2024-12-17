#ifndef SERVER_CLIENT_SESSION
#define SERVER_CLIENT_SESSION

#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <syslog.h>

#include "../common/common_socket.h"
#include "../common/common_thread.h"
#include "../common/types/constants.h"
#include "types/client_identity.h"

#include "map_info.h"
#include "server_matches_monitor.h"
#include "server_protocol.h"
#include "server_receiver.h"
#include "server_sender.h"


#define CLIENT_QUEUE_SIZE 100

class ClientSession: public Thread {
private:
    Identity identity;
    Socket skt;
    ServerProtocol protocol;
    MatchesMonitor& matches_monitor;
    Queue<std::shared_ptr<Snapshot>> client_queue;

    std::map<std::string, MapInfo> matches;  // guarda el nombre del mapa, y su estructura

public:
    ClientSession(Socket _skt, MatchesMonitor& monitor);

    uint8_t get_id() const;

    void end_communication();

    void get_available_maps(std::list<std::string>& map_list);

    void run() override;

    void stop() override;

    void run_lobby_loop();

    void run_receiver_loop();

    Queue<GameloopMessage>* get_match_queue();

    void exec_lobby_action(char action, bool& success);
};

#endif  // SERVER_CLIENT_SESSION
