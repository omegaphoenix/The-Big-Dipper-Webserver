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

// TODO: Replace old Handler.
// Note that the below code does not comply with 
// the common API and should be removed once the
// tests are reconfigured.

class StaticFileHandler: public Handler {
    public:
        StaticFileHandler(std::string requestPath, std::string basePath); 

        std::string handleRequests(std::string request);

    private:
        std::string requestPath;
        std::string basePath;
};
#endif
