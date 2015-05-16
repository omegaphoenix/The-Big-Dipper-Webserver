#include "static_file_handler.h"

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
