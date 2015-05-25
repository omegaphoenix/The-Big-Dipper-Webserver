#include "gtest/gtest.h"
#include "utils.h"

#include "webserver.h"

#include <sstream>
#include <string>

/*
 * Original example test included in assignment.
 */
TEST(NginxConfigParserTest, SimpleConfig) {
    NginxConfigParser parser;
    NginxConfig out_config;

    bool success = parser.Parse("example_config", &out_config);

    EXPECT_TRUE(success);
}

/*
 * Test for toString written in class.
 */
TEST(NginxConfigStatementTest, ToString) {
    NginxConfigStatement statement;
    statement.tokens_.push_back("foo");
    statement.tokens_.push_back("bar");
    EXPECT_EQ("foo bar;\n", statement.ToString(0));
}

class NginxStringConfigTest : public ::testing::Test {
protected:
    bool ParseString(const std::string &config_string) {
        std::stringstream config_stream(config_string);
        return parser_.Parse(&config_stream, &config_);
    }
    int GetPort(const std::string &config_string) {
        ParseString(config_string);
        return getPort(config_);
    }

    NginxConfigParser parser_;
    NginxConfig config_;
};

/*
 * Test written in class.
 */
TEST_F(NginxStringConfigTest, SimpleTextConfig) {
    EXPECT_TRUE(ParseString("foo bar;"));
}

/*
 * Test written in class.
 */
TEST_F(NginxStringConfigTest, SimpleBadTextConfig) {
    EXPECT_FALSE(ParseString("foo bar"));
}

TEST_F(NginxStringConfigTest, NestedConfig) {
    EXPECT_TRUE(ParseString(
            "foo { "
            "   bar { "
            "       baz aux;"
            "   }"
            "}"));
}

TEST_F(NginxStringConfigTest, GetPortTest) {
    EXPECT_EQ(20, GetPort("listen 20;"));
}



// TODO: Change tests to conform to new RequestHandler.
class RequestHandlerTest : public ::testing::Test {
    // Added 5/7/2015
    // Assignment 5 unit tests

    // Modified 5/24/2015 to work with new handlers.
    protected:
    bool NewHandleHelloResponse(const HTTPRequest &req) {
        NewHelloHandler a; // changed
        std::string output;
        size_t check_http200;
        size_t check_content_type;
        size_t check_hello;
        size_t check_date;

        output = a.HandleRequest(req);
        check_http200 = output.find("HTTP/1.0 200 OK\r\n"); //note- could not use protected var.
        check_content_type = output.find("Content-Type: text/html;\r\n");  
        check_hello = output.find("\r\n<html><body>Hello, world!</body></html>\r\n");
        check_date = output.find("GMT");

        if (check_http200 == std::string::npos ||
            check_content_type == std::string::npos ||
            check_hello == std::string::npos || 
            check_date == std::string::npos) {
            return 0;
        }
        return 1;     
    }   

    bool NewHandleEchoResponse(const HTTPRequest &req) {
        NewEchoHandler * a = new NewEchoHandler();
        std::string output;
        size_t check_http200;
        size_t check_content_type;
        size_t check_date;
        size_t check_request;

        output = a->HandleRequest(req);
        check_http200 = output.find("HTTP/1.0 200 OK\r\n"); //note- could not use protected var.
        check_content_type = output.find("Content-Type: text/html;\r\n");  
        check_date = output.find("GMT");
        check_request = output.find(req.request_body);

        if (check_http200 == std::string::npos ||
            check_content_type == std::string::npos ||
            check_date == std::string::npos ||
            check_request == std::string::npos) {
            return 0;
        }
        return 1;     
    }  

    bool HandleStaticResponse(const HTTPRequest &req, 
            const std::string &expected, bool jpg_flag, bool http_flag) {
        NewStaticHandler * a = new NewStaticHandler();
        std::string output;
        size_t check_http;
        size_t check_content_type;
        size_t check_date;
        size_t check_request;

        output = a->HandleRequest(req);

        if (http_flag) {
            check_http = output.find("HTTP/1.0 200 OK\r\n");
        }
        else {
            check_http = output.find("HTTP/1.0 404 Not Found\r\n"); 
        }
        if (!jpg_flag) {
            check_content_type = output.find("Content-Type: text/html;\r\n");
        }
        else {
            check_content_type = output.find("Content-Type: image/jpeg;\r\n");
        }

        check_date = output.find("GMT");
        check_request = output.find(expected);

        if (check_http == std::string::npos ||
            check_content_type == std::string::npos ||
            check_date == std::string::npos ||
            check_request == std::string::npos) {
            return 0;
        }
        return 1;     
    }  
};

// Writing more tests, 5/7/2015 Assignment 5
// updated 5/25/2015 with new API

// creating HTTPRequests to be passed into test functions
HTTPRequest test_hello;
test_hello.method = "GET";
test_hello.path = "/hello";
test_hello.headers = NULL;
test_hello.request_body = "HTTP/1.1";

TEST_F(RequestHandlerTest, NewHandleHelloResponse) {
    EXPECT_TRUE(NewHandleHelloResponse(test_hello));
}


HTTPRequest test_echo;
test_echo.method = "GET";
test_echo.path = "/echo";
test_echo.headers = NULL;
test_echo.request_body = "HTTP/1.1";

TEST_F(RequestHandlerTest, NewHandleEchoResponse) {
    EXPECT_TRUE(NewHandleEchoResponse(test_echo));
}


HTTPRequest test_static1;
test_static1.method = "GET";
test_static1.path = "/static/test.txt";
test_static1.headers = NULL;
test_static1.request_body = "HTTP/1.1";

HTTPRequest test_static2;
test_static1.method = "GET";
test_static1.path = "/static/asdfsdf";
test_static1.headers = NULL;
test_static1.request_body = "HTTP/1.1";

HTTPRequest test_static3;
test_static1.method = "GET";
test_static1.path = "/static/Caltech.jpg";
test_static1.headers = NULL;
test_static1.request_body = "HTTP/1.1";

TEST_F(RequestHandlerTest, NewHandleStaticResponse) {
    // Depends on file /static_test/test.txt with content "testing".
    // Test also assumes the mapping m["/static"] = "./static_test"
    EXPECT_TRUE(HandleStaticResponse(test_static1, "testing", 0, 1));
    // Checks nonexistent file /static_test/asdfsdf 
    EXPECT_TRUE(HandleStaticResponse(test_static2, "Error 404", 0, 0));
    //
    EXPECT_TRUE(HandleStaticResponse(test_static3, "jpeg", 1, 1));
}



