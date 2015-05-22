#include "static_file_handler.h"

void NewStaticHandler::Configure(const NginxConfig& child_config_block) {
    // TODO: Initialize this->root.
    for (std::vector<std::shared_ptr<NginxConfigStatement>>::const_iterator iter =
         child_config_block.statements_.begin();
         iter != child_config_block.statements_.end(); ++iter) {
            if (((*iter)->tokens_.size() > 1)) {
                if((*iter)->tokens_[0] == "root")
                {
                    this->root = (*iter)->tokens_[1];
                }
            }
        } 
}

std::string NewStaticHandler::HandleRequest(const HTTPRequest& req) {
    // Strip handler extension.
    std::istringstream iss(req.path); 
    std::string handlerExt = "";
    std::getline(iss, handlerExt, '/');  
    std::getline(iss, handlerExt, '/'); 
    std::string path(req.path.begin() + handlerExt.size() + 1, req.path.end());
    path = "/" + path;

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

// TODO: Replace old Handler.
// Note that the below code does not comply with 
// the common API and should be removed once the
// tests and getMappings() are reconfigured, and 
// the new handlers are renamed (if necessary).

StaticFileHandler::StaticFileHandler(std::string handlerExt, std::string basePath) : Handler(handlerExt) {
    this->basePath = basePath;
}

std::string StaticFileHandler::handleRequests(std::string request) {
    std::string path = stripHandlerExt(request);

    // Check for index request cases. 
    if (path.empty()) {
        path = "/index.html";
    }
    else if (path[path.size() - 1] == '/') {
        path += "index.html";
    }

    // Get requested file.
    std::string full_path = basePath + path;
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
