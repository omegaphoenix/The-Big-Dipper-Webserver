#include <string>
#include <stdexcept>
#include <boost/asio.hpp>
#include "webserver.h"

using boost::asio::ip::tcp;

WebServer::WebServer(int port, std::map<std::string, Handler*> *handlerMap) {
    this->port = port;
    if (handlerMap == NULL) {
        throw std::invalid_argument("Invalid handlerMap given to server.");
    }
    else {
        this->handlerMap = handlerMap;
    }
}

int WebServer::getPort() {
    return port;
}


void WebServer::run() {
    try {
        boost::asio::io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));

        for (;;) {
            // ASIO syncrhonous socket set up.
            tcp::socket socket(io_service);
            acceptor.accept(socket);

            // Read request header.
            boost::system::error_code read_error;
            boost::asio::streambuf buffer;
            std::string request = "";
            std::string line = "";
            boost::asio::read_until(socket, buffer, "\r\n\r\n", read_error);
            std::istream str(&buffer);
            while (true) {
                std::getline(str, line);
                std::cout << line << '\n';
                request += line;
                if ((char) str.peek() == '\r') {
                    break;
                }
            }
            std::cout << "Request: " << request << '\n';
       
            // Define a few initial things
            std::string backslash = "/";
            std::string token = request.substr(request.find(backslash)); // will return substring after first backslash

            // now, we want to return the substring until the space or backslash      
            std::string token2 = token.substr(0, token.find(" "));

            // finally, there may be multiple backslashes. This will concatenate the string
            // further if there are. Uses the string after the first backslash.
            std::string token3 = token2.substr(1);

            // if another backslash exists, concatenate it and everything afterwards
            std::size_t found = token3.find(backslash);
            if (found != std::string::npos) {   
               token2 = token2.substr(0, found + 1);
            }
            std::cout << "Handler Extension: " << token2 << '\n';
   
            // Handle requests. 
            boost::system::error_code write_error;
            std::string response = "";
            if (handlerMap->count(token2) != 0) {
                response = (*handlerMap)[token2]->handleRequests(request);
            }
            std::cout << "Response: " <<  response << '\n';
            boost::asio::write(socket, boost::asio::buffer(response), write_error);
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

