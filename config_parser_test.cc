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
    EXPECT_EQ(20, GetPort("port port 20;"));
}



class RequestHandlerTest : public ::testing::Test {
    // Added 5/7/2015
    // Assignment 5 unit tests
    protected:
    bool HandleHelloResponse(const std::string &config_string) {
        HelloWorldHandler * a = new HelloWorldHandler("/hello");
        std::string output;
        size_t check_http200;
        size_t check_content_type;
        size_t check_hello;
        size_t check_date;

        output = a->handleRequests(config_string);
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

    bool HandleEchoResponse(const std::string &config_string) {
        EchoHandler * a = new EchoHandler("/echo");
        std::string output;
        size_t check_http200;
        size_t check_content_type;
        size_t check_date;
        size_t check_request;

        output = a->handleRequests(config_string);
        check_http200 = output.find("HTTP/1.0 200 OK\r\n"); //note- could not use protected var.
        check_content_type = output.find("Content-Type: text/html;\r\n");  
        check_date = output.find("GMT");
        check_request = output.find(config_string);

        if (check_http200 == std::string::npos ||
            check_content_type == std::string::npos ||
            check_date == std::string::npos ||
            check_request == std::string::npos) {
            return 0;
        }
        return 1;     
    }  

    bool HandleStaticResponse(const std::string &config_string, 
            const std::string &expected, bool jpg_flag, bool http_flag) {
        StaticFileHandler * a = new StaticFileHandler("/static", "./static_test");
        std::string output;
        size_t check_http;
        size_t check_content_type;
        size_t check_date;
        size_t check_request;

        output = a->handleRequests(config_string);

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
TEST_F(RequestHandlerTest, HandleHelloResponse) {
    EXPECT_TRUE(HandleHelloResponse("GET /hello HTTP/1.1"));
}

TEST_F(RequestHandlerTest, HandleEchoResponse) {
    EXPECT_TRUE(HandleEchoResponse("GET /echo HTTP/1.1"));
}

TEST_F(RequestHandlerTest, HandleStaticResponse) {
    // Depends on file /static_test/test.txt with content "testing".
    // Test also assumes the mapping m["/static"] = "./static_test"
    EXPECT_TRUE(HandleStaticResponse("GET /static/test.txt HTTP/1.1", "testing", 0, 1));
    // Checks nonexistent file /static_test/asdfsdf 
    EXPECT_TRUE(HandleStaticResponse("GET /static/asdfsdf HTTP/1.1", "Error 404", 0, 0));
    //
    EXPECT_TRUE(HandleStaticResponse("GET /static/Caltech.jpg HTTP/1.1", "jpeg", 1, 1));
}

