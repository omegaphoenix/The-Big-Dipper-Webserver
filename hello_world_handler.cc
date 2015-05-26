#include "hello_world_handler.h"

void HelloHandler::Configure(const NginxConfig& child_config_block) {
    // Does not need to configure.
}

std::string HelloHandler::HandleRequest(const HTTPRequest& req) {
    std::string date = makeDaytimeString();
    return http200 + contentTypeHTML + date + hello;
}


