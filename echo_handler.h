#ifndef ECHO_HANDLER_H
#define ECHO_HANDLER_H
#include "request_handler.h"

class NewEchoHandler : public RequestHandler {
    public:
        void Configure(const NginxConfig& child_config_block);
        std::string HandleRequest(const HTTPRequest &req);
};

// TODO: Replace old Handler.
// Note that the below code does not comply with 
// the common API and should be removed once the
// tests and getMappings() are reconfigured, and 
// the new handlers are renamed (if necessary).

class EchoHandler : public Handler {
    public:
        EchoHandler(std::string handlerExt);
        std::string handleRequests(std::string request);
};
#endif
