#include "echo_handler.h"

void NewEchoHandler::Configure(const NginxConfig& child_config_block) {
    // Echo handler doesn't need to configure.
}

std::string NewEchoHandler::HandleRequest(const HTTPRequest &req) {
    // Constructs a string with request and request headers.
    std::string content = req.method + " " + req.path + "\n";
    for (int i = 0; i < req.headers.size(); i++) {
        std::pair<std::string, std::string> header = req.headers[i];
        content += header.first + " " + header.second + "\n";
    }
    std::string date = makeDaytimeString();
    return http200 + contentTypeHTML + date + "\r\n" + content;
}

// TODO: Replace old Handler.
// Note that the below code does not comply with 
// the common API and should be removed once the
// tests and getMappings() are reconfigured, and 
// the new handlers are renamed (if necessary).

EchoHandler::EchoHandler(std::string handlerExt) :
    Handler(handlerExt) {}

std::string EchoHandler::handleRequests(std::string request) {
    std::string date = makeDaytimeString();
    return http200 + contentTypeHTML + date + "\r\n" + request;
}
