#ifndef HELLO_WORLD_HANDLER_H
#define HELLO_WORLD_HANDLER_H
#include "request_handler.h"

class NewHelloHandler: public RequestHandler {
    public:
        void Configure(const NginxConfig& child_config_block);
        std::string HandleRequest(const HTTPRequest& req);
    private:
        const std::string hello = "\r\n<html><body>"
                                  "Hello, world!</body></html>\r\n";
};


#endif // HELLO_WORLD_HANDLER_H
