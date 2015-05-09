#ifndef ECHO_HANDLER_H
#define ECHO_HANDLER_H
#include "request_handler.h"

class EchoHandler: public Handler {
    public:
        std::string handleRequests(std::string request);
};
#endif
