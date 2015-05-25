#define newAPI 1
#include "utils.h"

int getPort(const NginxConfig &config)
{
    for (const auto& statement : config.statements_) {
        bool k1 = false;
        for (const std::string& token : statement->tokens_) {
            if (k1) {
                try {
                    return stoi(token);
                }
                catch (...) {
                }
            }
#if newAPI
            k1 = (token == "listen");
#else
            k1 = (token == "port");
#endif
        }
    }
    return -1;
}

std::map<std::string, RequestHandler*> *getMappings(const NginxConfig &config)
{
    std::map<std::string, RequestHandler*> *handlerMap = new std::map<std::string, RequestHandler*>;
#if newAPI
    for(const auto& statement: config.statements_) {
        std::string name = "";
        RequestHandler *h;
        bool k = false; // True if last token was "handler"
        bool k1 = false; // True if last two tokens were handler helloworld
        bool k2 = false; // True if last two tokens were handler copycat
        bool k3 = false; // True if past handler static but not root
        for (const std::string& token : statement->tokens_) {
            if (k) {
                k1 = (token == "helloworld");
                k2 = (token == "echo");
                k3 = (token == "static");
            }
            else {
                if (k1) {
                    try {
                        printf("Created helloworld\n");
                        h = new NewHelloHandler();
                        handlerMap->insert(std::pair<std::string, RequestHandler*>(token, h));
                    }
                    catch (...) {
                    }
                }
                else if (k2) {
                    try {
                        printf("Created echo\n");
                        h = new NewEchoHandler();
                        handlerMap->insert(std::pair<std::string, RequestHandler*>(token, h));
                    }
                    catch (...) {
                    }
                }
                else if (k3) {
                    name = token;
                    printf("Created static\n");
                    try {
                        h = new NewStaticHandler();
                        h->Configure(*(statement->child_block_));
                        handlerMap->insert(std::pair<std::string, RequestHandler*>(name, h));
                    }
                    catch (...) {
                    }
                }
                k1 = false;
                k2 = false;
                k3 = false;
            }
            k = (token == "handler");
        }
    }
#else
    for(const auto& statement: config.statements_) {
        std::string name = "";
        Handler *h;
        bool k1 = false; // Was last token helloWorld?
        bool k2 = false; // Was last token echo?
        bool k3 = false; // Was last token static?
        bool k4 = false; // Was last last token static?
        for (const std::string& token : statement->tokens_) {
            if (k1) {
                try {
                    h = new HelloWorldHandler(token);
                    handlerMap->insert(std::pair<std::string, Handler*>(token, h));
                }
                catch (...) {
                }
            }
            else if (k2) {
                try {
                    h = new EchoHandler(token);
                    handlerMap->insert(std::pair<std::string,Handler*>(token, h));
                }
                catch (...) {
                }
            }
            else if (k3) {
                try {
                    name = token;
                }
                catch (...) {
                }
            }
            else if (k4) {
                try {
                    h = new StaticFileHandler(name, token);
                    handlerMap->insert(std::pair<std::string,Handler*>(name, h));
                }
                catch (...) {
                }
            }
            k1 = (token == "helloWorld");
            k2 = (token == "echo");
            k4 = k3;
            k3 = (token == "static");
        }
    }
#endif
    return handlerMap;
}
