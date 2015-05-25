#include "hello_world_handler.h"

void NewHelloHandler::Configure(const NginxConfig& child_config_block) {
    // Does not need to configure.
}

std::string NewHelloHandler::HandleRequest(const HTTPRequest& req) {
    std::string date = makeDaytimeString();
    return http200 + contentTypeHTML + date + hello;
}


// TODO: Replace old Handler.
// Note that the below code does not comply with 
// the common API and should be removed once the
// tests are reconfigured. 

HelloWorldHandler::HelloWorldHandler(std::string handlerExt) 
    : Handler(handlerExt) {}

std::string HelloWorldHandler::handleRequests(std::string request) {
    std::string date = makeDaytimeString();
    return http200 + contentTypeHTML + date + hello;
}
