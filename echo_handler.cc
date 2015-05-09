#include "echo_handler.h"

std::string EchoHandler::handleRequests(std::string request) {
    std::string date = makeDaytimeString();
    return http200 + contentTypeHTML + date + "\r\n" + request;
}
