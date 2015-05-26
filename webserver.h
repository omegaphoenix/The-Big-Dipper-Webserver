#ifndef WEBSERVER_H
#define WEBSERVER_H
#include <iostream>
#include <string>
#include <map>
#include <boost/asio.hpp>
#include "request_handler.h"
#include "hello_world_handler.h"
#include "echo_handler.h"
#include "static_file_handler.h"
#include "error_handler.h"

using boost::asio::ip::tcp;

// Sets up a server
class WebServer {
    public:
        WebServer(int port = 80, 
                std::map<std::string, RequestHandler*> *handlerMap = NULL);
        int getPort();
        void parseHTTP(tcp::socket *socket, HTTPRequest *req);
        void run();
    private:
        int port;
        std::map<std::string, RequestHandler*> *handlerMap; 
        const std::string http200 = "HTTP/1.0 200 OK\n";
        const std::string contentType = "Content-Type: text/html;\n";
};
#endif
