#ifndef ECHO_HANDLER_H
#define ECHO_HANDLER_H
#include "request_handler.h"

class EchoHandler : public RequestHandler {
    public:
        void Configure(const NginxConfig& child_config_block);
        std::string HandleRequest(const HTTPRequest &req);
};

#endif // ECHO_HANDLER_H
