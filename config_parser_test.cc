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

// Added 5/7/2015
// Assignment 5 unit tests

    int HandleHelloResponse(const std::string &config_string) {
        HelloWorldHandler * a = new HelloWorldHandler();
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
            return 1;
        }
        return 0;     
    }   

    int HandleEchoResponse(const std::string &config_string) {
        EchoHandler * a = new EchoHandler();
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
            return 1;
        }
        return 0;     
    }  
/*
    int HandleStaticResponse(const std::string &config_string) {
        StaticFileHandler * a = new StaticFileHandler();
        std::string output;
        size_t check_http200;
        size_t check_content_type;
        size_t check_content_type2;
        size_t check_date;
        size_t check_request;

        output = a->handleRequests(config_string);
        check_http200 = output.find("HTTP/1.0 200 OK\r\n"); //note- could not use protected var.
        check_http404 = output.find("HTTP/1.0 404 Not Found\r\n"); // did not use
        check_content_type = output.find("Content-Type: text/html;\r\n");  
        check_content_type2 = output.find("Content-Type: image/jpeg;\r\n");
        check_date = output.find("GMT");

        // TODO: Need to do content checking

        if (check_http200 == std::string::npos ||
            check_content_type == std::string::npos ||
            check_date == std::string::npos) {
            return 1;
        }
        return 0;     
    }  
*/
    NginxConfigParser parser_;
    NginxConfig config_;
};
// ------------ //

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


// Writing more tests, 5/7/2015 Assignment 5

TEST_F(NginxStringConfigTest, HandleHelloResponse) {
    EXPECT_EQ(0, HandleHelloResponse("GET /hello HTTP/1.1"));
}

TEST_F(NginxStringConfigTest, HandleEchoResponse) {
    EXPECT_EQ(0, HandleEchoResponse("GET /echo HTTP/1.1"));
}

/*
TEST_F(NginxStringConfigTest, HandleStaticResponse) {
    EXPECT_EQ(0, HandleStaticResponse("GET /static HTTP/1.1"));
}

*/




