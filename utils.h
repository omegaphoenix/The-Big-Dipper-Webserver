#include <map>
#include <string>
#include "config_parser.h"

int getPort(const NginxConfig &config)
{
    for (const auto& statement : config.statements_) 
    {
        bool k1 = true;
        for (const std::string& token : statement->tokens_)
        {
            if (!k1)
            {
                try
                {
                    return stoi(token);
                }
                catch (...)
                {
                }
            }
            k1 = (token != "port");
        }
    }
    return -1;
}

std::map<std::string, std::string> getMappings(const NginxConfig &config)
{
    std::map<std::string, std::string> handlerMap;
    for(const auto& statement: config.statements_)
    {
        bool k1 = true;
        bool k2 = true;
        bool k3 = true;
        for (const std::string& token : statement->tokens_)
        {
            if (!k1)
            {
                try
                {
                    // TODO Create handler.
                    std::string s = "HelloWorldHandler";
                    handlerMap.insert(std::pair<std::string,std::string>(token,s));
                }
                catch (...)
                {
                }
            }
            else if (!k2)
            {
                try
                {
                    // TODO Create handler.
                    std::string s = "EchoHandler";
                    handlerMap.insert(std::pair<std::string,std::string>(token,s));
                }
                catch (...)
                {
                }
            }
            else if (!k3)
            {
                try
                {
                    // TODO Create handler. Also store next token as path.
                    std::string s = "StaticFileHandler";
                    handlerMap.insert(std::pair<std::string,std::string>(token,s));
                }
                catch (...)
                {
                }
            }
            k1 = (token != "helloWorld");
            k2 = (token != "echo");
            k3 = (token != "static");
        }
    }
    return handlerMap;
}
