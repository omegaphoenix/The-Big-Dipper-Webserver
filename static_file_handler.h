#ifndef STATIC_FILE_HANDLER_H
#define STATIC_FILE_HANDLER_H
#include "request_handler.h" 

class NewStaticHandler : public RequestHandler {
    public:
        void Configure(const NginxConfig& child_config_block);
        std::string HandleRequest(const HTTPRequest& req);
    private:
        std::string root;
};

#endif // STATIC_FILE_HANDLER_H
