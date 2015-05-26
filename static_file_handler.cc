#include "static_file_handler.h"

void StaticHandler::Configure(const NginxConfig& child_config_block) {
    for (std::vector<std::shared_ptr<NginxConfigStatement>>::const_iterator iter =
         child_config_block.statements_.begin();
         iter != child_config_block.statements_.end(); ++iter) {
            if (((*iter)->tokens_.size() > 1)) {
                if((*iter)->tokens_[0] == "root") {
                    this->root = (*iter)->tokens_[1];
                }
            }
        } 
}

std::string StaticHandler::HandleRequest(const HTTPRequest& req) {
    // Strip handler extension.
    std::istringstream iss(req.path); 
    std::string handlerExt = "";
    std::getline(iss, handlerExt, '/');  
    std::getline(iss, handlerExt, '/'); 
    std::string path(req.path.begin() + handlerExt.size() + 1, req.path.end());
    if (path.empty()) {
        path = "/";
    }

    // Check for index request. 
    if (path[path.size() - 1] == '/') {
        path += "index.html";
    }

    // Get requested file.
    std::string full_path = root + path;
    std::cout << "Searching for " + full_path + " ...\n";
    std::string content = "";
    std::ifstream file(full_path);
    if (file.is_open()) {
        std::string line = "";
        while (getline(file, line)) {
            content += line + "\n";
        }
        file.close();
    }

    std::string date = makeDaytimeString();

    std::string contentType;
    if (!content.empty() && path.find(".jpg") == path.size() - 4) {
        contentType = contentTypeJPEG;
    }
    else {
        contentType = contentTypeHTML;
    }

    if (content.empty()) {
        return http404 + contentType + date + "\r\n"
            "Error 404: Not Found.";
    }
    return http200 + contentType + date + "\r\n" + content;
}


