#include <string>
#include <sstream>
#include <stdexcept>
#include <boost/asio.hpp>
#include "webserver.h"

using boost::asio::ip::tcp;

WebServer::WebServer(int port, std::map<std::string, RequestHandler*> *handlerMap) {
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

void WebServer::parseHTTP(tcp::socket *socket, HTTPRequest *req) {
    // Read request header.
    boost::system::error_code read_error;
    boost::asio::streambuf buffer;
    std::string line = "";
    boost::asio::read_until(*socket, buffer, "\r\n\r\n", read_error);
    std::istream socket_stream(&buffer);


    std::getline(socket_stream, line);
    std::istringstream iss(line);
    iss >> req->method;
    iss >> req->path;
    std::cout << "    Request:\n" << req->method + " " + req->path << '\n';

    bool has_content = false;
    int content_length = 0;

    while (true) {
        std::pair<std::string, std::string> header;
        std::getline(socket_stream, line);
        iss.clear();
        iss.str(line);
        iss >> header.first;
        iss >> header.second;
        req->headers.push_back(header);
        if (header.first.compare("Content-Length:") == 0) {
            has_content = true;
            content_length = std::stoi(header.second, NULL);
        }
        std::cout << header.first + " " + header.second << '\n';
        if ((char) socket_stream.peek() == '\r') {
            socket_stream >> line;
            break;
        }
    }

    if (has_content) {
        char *buffer = new char[content_length];
        socket_stream.get(buffer, content_length);
        std::string body(buffer);
        req->request_body = body;
        std::cout << body << '\n';
    }

    std::cout << '\n';
}


void WebServer::run() {
    try {
        std::cout << "\nStarting webserver... \n\n";
        boost::asio::io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));

        for (;;) {
            // ASIO syncrhonous socket set up.
            tcp::socket socket(io_service);
            acceptor.accept(socket);

            // Parse HTTP Request. 
            HTTPRequest req;
            parseHTTP(&socket, &req);
            // Might not need this line any more.
            std::istringstream iss(req.path);
            std::string handlerExt = "";
            std::getline(iss, handlerExt, '/'); // Remove initial '/' in path
            std::getline(iss, handlerExt, '/'); // Extract extension.
            handlerExt = "/" + handlerExt;

            // Handle requests. 
            boost::system::error_code write_error;
            std::string response = "";
            if (handlerMap->count(handlerExt) != 0) {
                response = (*handlerMap)[handlerExt]->HandleRequest(req);
            }
            std::cout << "    Response:\n" <<  response << "\n";
            boost::asio::write(socket, boost::asio::buffer(response), write_error);
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

