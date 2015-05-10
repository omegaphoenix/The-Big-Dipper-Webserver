#include "request_handler.h"

Handler::Handler(std::string handlerExt) {
    this->handlerExt = handlerExt;
}

std::string Handler::makeDaytimeString() {
    using namespace std;
    time_t now = time(0);
    char *c_str = asctime(gmtime(&now));
    std::string time_str(c_str);
    time_str.pop_back(); // Erase newline
    return "Date: " + time_str + " GMT\r\n";
}

std::string Handler::stripHandlerExt(std::string request) {
    std::istringstream iss(request);
    std::string path;
    iss >> path; // path = GET
    iss >> path; // path = /ext/foo/bar
    path.erase(0, handlerExt.size()); // path = /foo/bar
    return path;
}

