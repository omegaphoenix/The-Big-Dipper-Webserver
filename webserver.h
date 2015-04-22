#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

// Sets up a server
class WebServer {
    public:
        WebServer(int port = 80);
        int getPort();
        void helloWorld();
    private:
        int port;
        boost::asio::io_service io_service;
        std::string http200 = "HTTP/1.0 200 OK\n";
        std::string contentType = "Content-Type: text/html;\n";
        std::string hello = "\n<html><body>Hello, world!</body></html>\n";
};
