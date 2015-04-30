#include "gtest/gtest.h"
#include "utils.h"
#include "webserver.h"

#include <sstream>
#include <string>
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

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
 * Based off of client.cpp
 * Distributed under the Boost Software License, Version 1.0.
 * Currently only passes when server is running.
 */
TEST(HelloWorldTest, ClientTest) {
    //WebServer webserver;
    //webserver.helloWorld();
    boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);
    // Construct query using name of server and service
    tcp::resolver::query query("localhost", "80");
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::socket socket(io_service);
    // Try connecting to server
    boost::asio::connect(socket,endpoint_iterator);
    for (;;)
    {
        boost::array<char, 128> buf;
        boost::system::error_code error;
        // Send get request
        std::stringstream request_;
        request_ << "GET / HTTP/1.1\r\n";
        request_ << "\r\n";
        boost::asio::write(socket, boost::asio::buffer(request_.str()), error);
        // Read reply from server
        size_t len = socket.read_some(boost::asio::buffer(buf), error);
        if (error == boost::asio::error::eof)
        {
            break;
        }
        else if (error)
        {
            throw boost::system::system_error(error);
        }
        std::cout.write(buf.data(), len);
    }
    // Returns true if no error is thrown
    EXPECT_TRUE(true);
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

// Test written in class.
TEST_F(NginxStringConfigTest, SimpleTextConfig) {
    EXPECT_TRUE(ParseString("foo bar;"));
}

// Test written in class.
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

