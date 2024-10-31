#ifndef SOCKET_WAS_CLOSED_EXCEPTION
#define SOCKET_WAS_CLOSED_EXCEPTION

#include "common_liberror.h"

class SocketWasCLosedException: public LibError {
    // Hereda el constructor de la clase LibError
    using LibError::LibError;
};

#endif  // SOCKET_WAS_CLOSED_EXCEPTION
