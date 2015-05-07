#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

// Sets up a server
class WebServer {
    public:
        WebServer(int port = 80);
        int getPort();
        std::string makeDaytimeString();
        void handleRequest();
        void helloWorld();
        void demo();
    private:
        int port;
        const std::string http200 = "HTTP/1.0 200 OK\n";
        const std::string contentType = "Content-Type: text/html;\n";
        // Should be in helloWorldHandler
        const std::string hello = "\n<html><body>"
                                  "Hello, world!</body></html>\n";
};
