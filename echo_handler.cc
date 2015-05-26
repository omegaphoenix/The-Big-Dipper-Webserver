#include "echo_handler.h"

void EchoHandler::Configure(const NginxConfig& child_config_block) {
    // Echo handler doesn't need to configure.
}

std::string EchoHandler::HandleRequest(const HTTPRequest &req) {
    // Constructs a string with request and request headers.
    std::string content = req.method + " " + req.path + "\n";
    for (unsigned int i = 0; i < req.headers.size(); i++) {
        std::pair<std::string, std::string> header = req.headers[i];
        content += header.first + " " + header.second + "\n";
    }
    std::string date = makeDaytimeString();
    return http200 + contentTypeHTML + date + "\r\n" + content;
}


