#include <iostream>

#include "server_class.h"

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            return EXIT_FAILURE;
        }

        std::string puerto = std::string(argv[1]);
        Server server(puerto);
        server.execute();
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        return EXIT_FAILURE;
    }
}
