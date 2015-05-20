// Based on Yichi's Hw 3 Solution

#include <fstream>
#include <iostream>
#include <boost/asio.hpp>

#include "webserver.h"
#include "utils.h"

int main(int argc, char* argv[]) {
    if (argc == 2) {
        NginxConfigParser config_parser;
        NginxConfig config;

        if (!config_parser.Parse(argv[1], &config)) {
            return -1;
        }

        int port = getPort(config);

        if (port > 0) {
            std::map<std::string, Handler*> *handler_map =
                getMappings(config);

            WebServer webserver(port, handler_map);
            webserver.run();
            return 0;
        }
    }
    else {
        std::cerr << "Usage: ./webserver [config_file]" << std::endl;
    }
    return -1;
}
