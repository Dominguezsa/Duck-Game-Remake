#ifndef SERVER_CLIENT_IDENTITY
#define SERVER_CLIENT_IDENTITY

#include <cstdint>
#include <string>

#define UNDEFINED_NAME ""

struct Identity {
    std::string name;
    std::string joined_match_name;
    uint8_t id;

    Identity(): name(UNDEFINED_NAME), joined_match_name(UNDEFINED_NAME), id(0) {}
};

#endif  // SERVER_CLIENT_IDENTITY
