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

void WebServer::helloWorld() {
    for (;;) {
        boost::asio::io_service io_service;
        
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));
        
        tcp::socket socket(io_service);
        acceptor.accept(socket);

        boost::system::error_code read_error;
        boost::asio::streambuf buffer;
        boost::asio::read_until(socket, buffer, "\n", read_error);
        std::istream str(&buffer);
        std::string request;
        std::getline(str, request);
        std::cout << request << '\n';

        if (request.find("GET") == 0) {
            boost::system::error_code write_error;
            boost::asio::write(socket, boost::asio::buffer(http200), write_error);
            boost::asio::write(socket, boost::asio::buffer(contentType), write_error);
            boost::asio::write(socket, boost::asio::buffer(hello), write_error);
        }
    }
}
