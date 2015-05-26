#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H
#include "request_handler.h"

class ErrorHandler : public RequestHandler {
    public:
        void Configure(const NginxConfig& child_config_block);
        std::string HandleRequest(const HTTPRequest &req);
};

#endif // ERROR_HANDLER_H
