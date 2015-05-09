#ifndef STATIC_FILE_HANDLER_H
#define STATIC_FILE_HANDLER_H
#include "request_handler.h" 
class StaticFileHandler: public Handler {
    public:
        StaticFileHandler(std::string requestPath, std::string basePath); 

        std::string handleRequests(std::string request);

    private:
        std::string requestPath;
        std::string basePath;
};
#endif
