#include "hello_world_handler.h"

HelloWorldHandler::HelloWorldHandler(std::string handlerExt) 
    : Handler(handlerExt) {}

std::string HelloWorldHandler::handleRequests(std::string request) {
    std::string date = makeDaytimeString();
    return http200 + contentTypeHTML + date + hello;
}
