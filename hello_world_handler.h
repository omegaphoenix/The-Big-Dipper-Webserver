#ifndef HELLO_WORLD_HANDLER_H
#define HELLO_WORLD_HANDLER_H
#include "request_handler.h"

class HelloWorldHandler: public Handler {
    public:
        HelloWorldHandler(std::string handlerExt);
        std::string handleRequests(std::string request);
    private:
        const std::string hello = "\r\n<html><body>" 
                                  "Hello, world!</body></html>\r\n";
};
#endif
