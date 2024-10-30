#include <cstdlib>
#include <string.h>
#include <arpa/inet.h>
#include <string.h>
#include <syslog.h>

#include "server_acceptor.h"

#define ERROR 1
#define SUCCESS 0

int main(int argc, char *argv[]) {
    const char *servname = NULL;
    try {
        if (argc == 2) {
            servname = argv[1];
            // ------- Inicio Server -------
            AcceptorThread server(servname);
            server.start();
            server.stop();
            server.join();
            return SUCCESS;
            //------------------------------
        } else {
            std::string error_msg = "Bad program call. Expected " + std::string(argv[0])
                                    + " <servname>\n";
            syslog(LOG_ERR, "%s", error_msg.c_str());
            return ERROR;
        }
    } catch (...) {
        syslog(LOG_ERR, "%s", "Something went wrong and an exception was caught: \n");
        return ERROR;
    }
}