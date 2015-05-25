#include "static_file_handler.h"

void ErrorHandler::Configure(const NginxConfig& child_config_block) {
}

std::string ErrorHandler::HandleRequest(const HTTPRequest& req) {
    std::string date = makeDaytimeString();
    return http404 + date + "\r\n"
            "Error 404: Not Found.";
   
}
