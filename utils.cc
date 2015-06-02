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
            k1 = (token == "listen");
        }
    }
    return -1;
}

std::map<std::string, RequestHandler*> *getMappings(const NginxConfig &config)
{
    std::map<std::string, RequestHandler*> *handlerMap = new std::map<std::string, RequestHandler*>;
    for(const auto& statement: config.statements_) {
        std::string name = "";
        RequestHandler *h;
        bool k = false; // True if last token was "handler"
        bool k1 = false; // True if last two tokens were handler helloworld
        bool k2 = false; // True if last two tokens were handler copycat
        bool k3 = false; // True if past handler static but not root
        bool k4 = false; // True if last two tokens were handler error
        bool k5 = false; // True if last two tokens were handler proxy 
        for (const std::string& token : statement->tokens_) {
            if (k) {
                k1 = (token == "helloworld");
                k2 = (token == "echo");
                k3 = (token == "static");
                k4 = (token == "error");
                k5 = (token == "proxy");
            }
            else {
                if (k1) {
                    try {
                        printf("Created helloworld handler.\n");
                        h = new HelloHandler();
                        h->Configure(*(statement->child_block_));
                        handlerMap->insert(std::pair<std::string, RequestHandler*>(token, h));
                    }
                    catch (...) {
                    }
                }
                else if (k2) {
                    try {
                        printf("Created echo handler.\n");
                        h = new EchoHandler();
                        h->Configure(*(statement->child_block_));
                        handlerMap->insert(std::pair<std::string, RequestHandler*>(token, h));
                    }
                    catch (...) {
                    }
                }
                else if (k3) {
                    name = token;
                    printf("Created static handler.\n");
                    try {
                        h = new StaticHandler();
                        h->Configure(*(statement->child_block_));
                        handlerMap->insert(std::pair<std::string, RequestHandler*>(name, h));
                    }
                    catch (...) {
                    }
                }
                else if (k4) {
                    try {
                        printf("Created error handler.\n");
                        h = new ErrorHandler();
                        h->Configure(*(statement->child_block_));
                        handlerMap->insert(std::pair<std::string, RequestHandler*>(token, h));
                    }
                    catch (...) {
                    }
                }
                else if (k5) {
                    try {
                        printf("Created proxy handler.\n");
                        h = new ProxyHandler();
                        h->Configure(*(statement->child_block_));
                        handlerMap->insert(std::pair<std::string, RequestHandler*>(token, h));
                    }
                    catch (...) {
                    }
                }

                k1 = false;
                k2 = false;
                k3 = false;
                k4 = false;
                k5 = false;
            }
            k = (token == "handler");
        }
    }
    return handlerMap;
}
