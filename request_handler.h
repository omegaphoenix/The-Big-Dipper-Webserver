#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H 

/*
 * The RequestHandler class and HTTPRequest struct are
 * part of the common API discussed in CS 3 class 5/15/15.
 * Credit goes to Josh Hyman for supplying this API design.
 * See https://github.com/Caltech-CS3-2015/webserver-api/blob/master/request_handler.h
 */

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <boost/asio.hpp>
#include "config_parser.h"

using boost::asio::ip::tcp;

struct HTTPRequest {
    // The method of request (e.g. GET, HEAD, etc).
    std::string method;

    // The path (e.g. /path/to/handler/index.html).
    std::string path;

    // Parsed headers (in order).
    std::vector<std::pair<std::string, std::string>> headers;

    // The body of the request.
    std::string request_body;
};

// RequestHandler is a long lived object that is expected to be created at
// server initialization time. There should be one RequestHandler for each
// config block that will be stored in a dispatch map (see above).
//
// Expected setup:
//   RequestHandler* h = new MyDerivedHandler(...);
//   h->Configure(*config_.statements_[block_i]->child_block_.get())
//   dispatch[path] = h;
class RequestHandler {
    public:
        virtual ~RequestHandler() {}

        // Configures this request handler to handle a particular type of request.
        // It will be passed only the block of the config that pertains to this
        // handler. The config block is represented by an NginxConfig.
        //
        // This must be called before calling HandleRequest().
        virtual void Configure(const NginxConfig& child_config_block) = 0;

        // Handles and individual request. Returns the response, including the
        // return code, headers, and body.
        virtual std::string HandleRequest(const HTTPRequest& req) = 0;
        
        // Gets the current time in GMT. 
        std::string makeDaytimeString();

    protected:
        // Helpful HTTP constants.
        const std::string http200 = "HTTP/1.0 200 OK\r\n";
        const std::string http404 = "HTTP/1.0 404 Not Found\r\n";
        const std::string contentTypeHTML = "Content-Type: text/html;\r\n";
        const std::string contentTypeJPEG = "Content-Type: image/jpeg;\r\n";
};


#endif // REQUEST_HANDLER_H
