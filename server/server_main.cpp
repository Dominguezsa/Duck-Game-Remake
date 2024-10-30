#include "server_class.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        return EXIT_FAILURE;
    }
    std::string puerto = std::string(argv[1]);
    Server server(puerto);
    server.execute();
    return EXIT_SUCCESS;
}