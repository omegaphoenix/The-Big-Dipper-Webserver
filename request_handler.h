#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Handler {
    public:
        virtual std::string handleRequests(std::string request) = 0;
};

class HelloWorldHandler: public Handler {
    public:
        std::string handleRequests(std::string request) {
            return hello;
        }
    private:
        const std::string hello = "\n<html><body>" 
                                  "Hello, world!</body></html>\n";
};

class EchoHandler: public Handler {
    public:
        std::string handleRequests(std::string request) {
            return request;
        }
};

class StaticFileHandler: public Handler {
    public:
        std::string handleRequests(std::string request) {
            return NULL;
        }
};
