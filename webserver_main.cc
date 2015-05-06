// Based on Yichi's Hw 3 Solution

#include <fstream>
#include <iostream>
#include <boost/asio.hpp>

#include "utils.h"
#include "webserver.h"

int main(int argc, char* argv[]) {
    if(argc == 2)
    {
        NginxConfigParser config_parser;
        NginxConfig config;
        if (!config_parser.Parse(argv[1], &config))
        {
            return -1;
        }
        int port = getPort(config);
        //int port = 80;
        if(port > 0)
        {
            //WebServer webserver;
            WebServer webserver(port);
            webserver.helloWorld();
            //webserver.demo();
            return 0;
        }
    }
    else
    {
        std::cerr << "Usage: ./webserver [config_file]" << std::endl;
    }
    return -1;
}
