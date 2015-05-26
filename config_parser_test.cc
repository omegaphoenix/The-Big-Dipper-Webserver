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


class RequestHandlerTest : public ::testing::Test {
    // Added 5/7/2015
    // Assignment 5 unit tests

    // Modified 5/24/2015 to work with new handlers.
    protected:
    bool HandleHelloResponse(const HTTPRequest &req) {
        HelloHandler a; // changed
        std::string output;
        size_t check_http200;
        size_t check_content_type;
        size_t check_hello;
        size_t check_date;

        output = a.HandleRequest(req);
        check_http200 = output.find("HTTP/1.0 200 OK\r\n"); 
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

    bool HandleEchoResponse(const HTTPRequest &req) {
        EchoHandler a;
        std::string output;
        size_t check_http200;
        size_t check_content_type;
        size_t check_date;
        size_t check_request;

        output = a.HandleRequest(req);
        check_http200 = output.find("HTTP/1.0 200 OK\r\n"); 
        check_content_type = output.find("Content-Type: text/html;\r\n");  
        check_date = output.find("GMT");
        check_request = output.find(req.method + " " + req.path + "\n");

        if (check_http200 == std::string::npos ||
            check_content_type == std::string::npos ||
            check_date == std::string::npos ||
            check_request == std::string::npos) {
            return 0;
        }
        return 1;     
    }  

    bool HandleStaticResponse(const HTTPRequest &req, 
            const std::string &root_path, const std::string &expected, 
            bool jpg_flag, bool http_flag) {
        StaticHandler a;
        std::string output;
        size_t check_http;
        size_t check_content_type;
        size_t check_date;
        size_t check_request;

        NginxConfig config;
        std::shared_ptr<NginxConfigStatement> statement (new NginxConfigStatement);
        statement->tokens_.push_back("root");
        statement->tokens_.push_back(root_path);
        config.statements_.push_back(statement);
        a.Configure(config);

        output = a.HandleRequest(req);

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

    bool HandleErrorResponse(const HTTPRequest &req) {
        ErrorHandler a; 
        std::string output;
        size_t check_http404;
        size_t check_date;
        size_t check_msg;

        output = a.HandleRequest(req);
        check_http404 = output.find("HTTP/1.0 404 Not Found\r\n"); 
        check_date = output.find("GMT");
        check_msg = output.find("Error 404: Not Found.");

        if (check_http404 == std::string::npos ||
            check_date == std::string::npos ||
            check_msg == std::string::npos) {
            return 0;
        }
        return 1;     
    }   
};

// Request Handler Tests:
TEST_F(RequestHandlerTest, HandleHelloResponse) {
    HTTPRequest test_hello;
    test_hello.method = "GET";
    test_hello.path = "/hello";
    EXPECT_TRUE(HandleHelloResponse(test_hello));
}


TEST_F(RequestHandlerTest, HandleEchoResponse) {
    HTTPRequest test_echo;
    test_echo.method = "GET";
    test_echo.path = "/echo";
    EXPECT_TRUE(HandleEchoResponse(test_echo));
}


TEST_F(RequestHandlerTest, HandleStaticResponse) {
    // Depends on file /static_test/test.txt with content "testing".
    // Test also assumes the mapping m["/static"] = "./static_test"
    HTTPRequest test_static1;
    test_static1.method = "GET";
    test_static1.path = "/static/test.txt";
    EXPECT_TRUE(HandleStaticResponse(test_static1, "./static_test", "testing", 0, 1));

    // Checks nonexistent file /static_test/asdfsdf 
    HTTPRequest test_static2;
    test_static2.method = "GET";
    test_static2.path = "/static/asdfsdf";
    EXPECT_TRUE(HandleStaticResponse(test_static2, "./static_test", "Error 404", 0, 0));

    // Depends on file /static_test/Caltech.jpg
    HTTPRequest test_static3;
    test_static3.method = "GET";
    test_static3.path = "/static/Caltech.jpg";
    EXPECT_TRUE(HandleStaticResponse(test_static3, "./static_test", "jpeg", 1, 1));
}


TEST_F(RequestHandlerTest, HandleErrorResponse) {
    HTTPRequest test_error;
    test_error.method = "GET";
    test_error.path = "/error";

    EXPECT_TRUE(HandleErrorResponse(test_error));
}


