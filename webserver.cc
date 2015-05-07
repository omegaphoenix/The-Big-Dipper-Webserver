#include <string>
#include <boost/asio.hpp>
#include "webserver.h"

using boost::asio::ip::tcp;

WebServer::WebServer(int port) {
    this->port = port;
}

int WebServer::getPort() {
    return port;
}

// GMT Timestamp
std::string WebServer::makeDaytimeString() {
    using namespace std;
    time_t  now = time(0); 
    char *c_str = asctime(gmtime(&now));
    std::string time_str(c_str);
    time_str.pop_back(); // Erase newline
    return "Date: " + time_str + " GMT\n";
}

void WebServer::handleRequest() {
    try {
        boost::asio::io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));

        for (;;) {
            // ASIO syncrhonous socket set up.
            tcp::socket socket(io_service);
            acceptor.accept(socket);
            // Read request.
            boost::system::error_code read_error;
            boost::asio::streambuf buffer;
            std::string request = "";

            //for (int i = 0; i < 7; i++) {
            boost::asio::read_until(socket, buffer, "\r\n\r\n", read_error);
            std::istream str(&buffer);
            std::getline(str, request);
            std::cout << request << '\n';
            //request += line + "<br/>";
            //}
            Handler *h = NULL;
            createHandler(request, &h);
            // Read different requests. TODO
            //response_handler::readRequests(socket, request);

            // Handle requests. TODO
            boost::system::error_code write_error;
            std::string get = h->handleRequests(request);
            // Echo GET request.
            if (request.find("GET") == 0) {
                std::string date = makeDaytimeString();
                //std::string content = http200 + contentType + date + hello;
                //std::string content = http200 + contentType + date 
                //    + "\n<html><body>" + request + "</body></html>\n";
                std::string content = http200 + contentType + date + get;
                boost::asio::write(socket, boost::asio::buffer(content), write_error);
            }
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void WebServer::createHandler(std::string request, Handler **h) {
    if (request.find("GET") == 0) {
        if (request.find("/echo") == 4) {
            *h = new EchoHandler;
        }
        else if (request.find("/") == 4) {
            *h = new HelloWorldHandler;
        }
        else if (request.find("/static") == 4) {
            *h = new StaticFileHandler;
        }
        else {
            printf("Failed to create Handler %d \n",request.find("/"));
        }
    }
}
