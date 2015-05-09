#include "static_file_handler.h"

StaticFileHandler::StaticFileHandler(std::string requestPath, std::string basePath) {
    this->requestPath = requestPath;
    this->basePath = basePath;
}

std::string StaticFileHandler::handleRequests(std::string request) {
    // Strip GET to form the std::string path 
    std::istringstream iss(request);
    std::string path;
    iss >> path; // Remove GET
    iss >> path;
    // Remove request path.
    // E.g. remove "/static" in GET /static/foo.html
    std::cout << "path: "  << path << '\n';
    path.erase(0, requestPath.size()); 

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
