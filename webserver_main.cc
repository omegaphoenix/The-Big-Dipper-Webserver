#include "webserver.h"
#include "config_parser.h"

int main(int argc, char* argv[]) {
    NginxConfigParser config_parser;
    NginxConfig config;
    int port;
    // Sets port to number in config file.
    config_parser.Parse(argv[1], &config, port);

    WebServer webserver(port);
    webserver.helloWorld();
    return 0;
}
