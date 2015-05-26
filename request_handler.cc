#include "request_handler.h"

std::string RequestHandler::makeDaytimeString() {
    using namespace std;
    time_t now = time(0);
    char *c_str = asctime(gmtime(&now));
    std::string time_str(c_str);
    time_str.pop_back(); // Erase newline
    return "Date: " + time_str + " GMT\r\n";
}


