#ifndef UTILS_H
#define UTILS_H
#include <map>
#include <string>
#include "config_parser.h"
#include "request_handler.h"
#include "hello_world_handler.h"
#include "echo_handler.h"
#include "static_file_handler.h"

int getPort(const NginxConfig &config);

std::map<std::string, RequestHandler*> *getMappings(const NginxConfig &config);

#endif // UTILS_H
