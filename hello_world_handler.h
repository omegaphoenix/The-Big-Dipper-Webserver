#ifndef HELLO_WORLD_HANDLER_H
#define HELLO_WORLD_HANDLER_H
#include "request_handler.h"

class NewHelloHandler: public RequestHandler {
    void Configure(const NginxConfig& child_config_block);
    std::string HandleRequest(const HTTPRequest& req);
    private:
        const std::string hello = "\r\n<html><body>"
                                  "Hello, world!</body></html>\r\n";
};

// TODO: Replace old Handler.
// Note that the below code does not comply with 
// the common API and should be removed once the
// tests and getMappings() are reconfigured, and 
// the new handlers are renamed (if necessary).


class HelloWorldHandler: public Handler {
    public:
        HelloWorldHandler(std::string handlerExt);
        std::string handleRequests(std::string request);
    private:
        const std::string hello = "\r\n<html><body>" 
                                  "Hello, world!</body></html>\r\n";
};
#endif
