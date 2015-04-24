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
    std::string time_str(c_str); // Convert to std::string
    time_str.pop_back(); // Erase newline
    return "Date: " + time_str + " GMT\n";
}

// Basic functionality
void WebServer::helloWorld() {
    for (;;) {
        // ASIO synchronous socket set up.
        boost::asio::io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));
        tcp::socket socket(io_service);
        acceptor.accept(socket);

        // Read request.
        boost::system::error_code read_error;
        boost::asio::streambuf buffer;
        boost::asio::read_until(socket, buffer, "\n", read_error);
        std::istream str(&buffer);
        std::string request;
        std::getline(str, request);
        std::cout << request << '\n';

        // Serve GET request.
        if (request.find("GET") == 0) {
            boost::system::error_code write_error;
            boost::asio::write(socket, boost::asio::buffer(http200), write_error);
            boost::asio::write(socket, boost::asio::buffer(contentType), write_error);
            std::string date = makeDaytimeString();
            boost::asio::write(socket, boost::asio::buffer(date), write_error);
            boost::asio::write(socket, boost::asio::buffer(hello), write_error);
        }
    }
}
