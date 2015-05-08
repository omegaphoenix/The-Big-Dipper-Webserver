#ifndef WEBSERVER_H
#define WEBSERVER_H
#include <iostream>
#include <string>
#include <map>
#include <boost/asio.hpp>
#include "request_handler.h"

using boost::asio::ip::tcp;

// Sets up a server
class WebServer {
    public:
        WebServer(int port = 80, 
                std::map<std::string, Handler*> *handlerMap = NULL);
        int getPort();
        void handleRequest();
    private:
        void createHandler(std::string request, Handler **h);
        int port;
        std::map<std::string, Handler*> *handlerMap; 
        const std::string http200 = "HTTP/1.0 200 OK\n";
        const std::string contentType = "Content-Type: text/html;\n";
};
#endif
