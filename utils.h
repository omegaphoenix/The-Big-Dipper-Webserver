#include "config_parser.h"

int getPort(const NginxConfig &config)
{
    for(const auto& statement : config.statements_) 
    {
        bool k1 = true;
        for(const std::string& token : statement->tokens_)
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

