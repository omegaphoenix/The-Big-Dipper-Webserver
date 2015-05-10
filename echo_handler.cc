#include "echo_handler.h"

EchoHandler::EchoHandler(std::string handlerExt) :
    Handler(handlerExt) {}

std::string EchoHandler::handleRequests(std::string request) {
    std::string date = makeDaytimeString();
    return http200 + contentTypeHTML + date + "\r\n" + request;
}
