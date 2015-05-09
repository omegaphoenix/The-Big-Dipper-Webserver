#include "hello_world_handler.h"

std::string HelloWorldHandler::handleRequests(std::string request) {
    std::string date = makeDaytimeString();
    return http200 + contentTypeHTML + date + hello;
}
