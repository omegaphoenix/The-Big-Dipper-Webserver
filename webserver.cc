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


//----------------------------------//

            // Define a few initial things
            std::string backslash = "/";
            std::string token = request.substr(request.find(backslash)); // will return substring after first backslash

            // now, we want to return the substring until the space or backslash      
            // NOTE: DEPENDS ON THE FACT THAT IT IS IMMEDIATELY FOLLOWED BY HTTP!!!
            std::string token2 = token.substr(0, token.find(" "));
            std::cout << "FINAL SUBSTRING:" << token2 << '\n';

            // finally, there may be multiple backslashes. This will concatenate the string
            // further if there are. Uses the string after the first backslash.
            std::string token3 = token2.substr(1);

            // if another backslash exists, concatenate it and everything afterwards
            std::size_t found = token3.find(backslash);
            if (found != std::string::npos) {   
               token2 = token2.substr(0, found + 1);
            }
            std::cout << "FINAL SUBSTRING:" << token2 << '\n';
   
//----------------------------------//

            Handler *h = NULL;
            createHandler(request, &h);
            // Read different requests. TODO
            //response_handler::readRequests(socket, request);

            // Handle requests. TODO
            boost::system::error_code write_error;
            std::string response = (*handlerMap)[token2]->handleRequests(request);

            std::cout << "Response:" <<  response << '\n';
            boost::asio::write(socket, boost::asio::buffer(response), write_error);
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
            std::cout << "Created echohandle\n";
        }
        else if (request.find("/hello") == 4) {
            *h = new HelloWorldHandler;
        }
        else if (request.find("/static") == 4) {
            *h = new StaticFileHandler("/static", "./static_test");
        }
        else {
            *h = new HelloWorldHandler;
            printf("Failed to create Handler %lu \n",request.find("/"));
        }
    }
}
