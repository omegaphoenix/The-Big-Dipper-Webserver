#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H
#include "request_handler.h"

class ErrorHandler : public RequestHandler {
    public:
        void Configure(const NginxConfig& child_config_block);
        std::string HandleRequest(const HTTPRequest &req);
};

#endif
/*
// This handler just reads the input request and returns a 404!
class ErrorHandler : public RequestHandler {
    public:
        ErrorHandler() {}
        virtual ~ErrorHandler() {}

        void Configure(const NginxConfig& child_config_block) {
            // Read in the URL of the image to show in the 404 page.
            for (auto stmt : child_config_block.statements_) {
                if (stmt.tokens_.size() == 2 && stmt.tokens_[0] == "image_url") {
                    image_url_ = stmt.tokens_[1];
                    return;
                }
            }
            cerr << "My ErrorHandler config is bad!" << child_config_block.ToString();
            cerr << "Could not find 'image_url' config statement.";
        }

        std::string HandleRequest(const HTTPRequest& req) {
            return CreateHTTPResponse(404, image_url_);
        }
    private:
        std::string CreateHTTPResponse(int error_code, std::string image_url) {
            return HTTPUtils::CreateHTTPResponse(error_code, Get404Page(image_url));
        }
        std::string image_url_;
}
*/
