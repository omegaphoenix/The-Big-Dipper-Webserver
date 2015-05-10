#ifndef ECHO_HANDLER_H
#define ECHO_HANDLER_H
#include "request_handler.h"

class EchoHandler: public Handler {
    public:
        EchoHandler(std::string handlerExt);
        std::string handleRequests(std::string request);
};
#endif
