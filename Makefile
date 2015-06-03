CXX = g++
CXXFLAGS = -std=c++0x -Wall -g
CPPFLAGS = -MMD
LDFLAGS = -lboost_system -lpthread
GTEST_DIR = gtest-1.7.0
TESTFLAGS = -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) -pthread
PROG = webserver config_parser_test proxy_handler_test 
SRCS = webserver.cc config_parser.cc request_handler.cc hello_world_handler.cc  echo_handler.cc error_handler.cc static_file_handler.cc proxy_handler.cc utils.cc

all: test webserver run_server
webserver: $(SRCS:.cc=.o) webserver_main.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

run_server:
	./webserver config_file 

test: config_parser_test proxy_handler_test
	./config_parser_test

config_parser_test: config_parser_test.o $(SRCS:.cc=.o)
	$(CXX) $(CXXFLAGS) $(TESTFLAGS) -c $(GTEST_DIR)/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o
	$(CXX) $(CXXFLAGS) $(TESTFLAGS) $(SRCS) config_parser_test.cc $(GTEST_DIR)/src/gtest_main.cc libgtest.a -o config_parser_test $(LDFLAGS)

proxy_handler_test: proxy_handler_test.o $(SRCS:.cc=.o) 
	$(CXX) $(CXXFLAGS) $(TESTFLAGS) -c $(GTEST_DIR)/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o
	$(CXX) $(CXXFLAGS) $(TESTFLAGS) $(SRCS) proxy_handler_test.cc $(GTEST_DIR)/src/gtest_main.cc libgtest.a -o proxy_handler_test $(LDFLAGS)

clean:
	$(RM) $(PROG) *.o *~

.PHONY: all clean run_server

-include $(SRCS:.cc=.d)
