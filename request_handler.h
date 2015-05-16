#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H 
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Handler {
    public:
        Handler(std::string handlerExt);
        virtual std::string handleRequests(std::string request) = 0;
        std::string makeDaytimeString();
        std::string stripHandlerExt(std::string request);
    protected:
        std::string handlerExt;
        const std::string http200 = "HTTP/1.0 200 OK\r\n";
        const std::string http404 = "HTTP/1.0 404 Not Found\r\n";
        const std::string contentTypeHTML = "Content-Type: text/html;\r\n";
        const std::string contentTypeJPEG = "Content-Type: image/jpeg;\r\n";
};

#endif
