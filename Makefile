CXX = g++
CXXFLAGS = -std=c++0x -Wall -g
OBJS = webserver.o config_parser.o webserver_main.o request_handler.o hello_world_handler.o echo_handler.o static_file_handler.o utils.o
LDFLAGS = -lboost_system
GTEST_DIR = gtest-1.7.0
TESTFLAGS = -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) -pthread

all: webserver test
webserver: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	./webserver config_file

test: config_parser_test
	./config_parser_test

config_parser_test:
	$(CXX) $(CXXFLAGS) $(TESTFLAGS) -c $(GTEST_DIR)/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o
	$(CXX) $(CXXFLAGS) $(TESTFLAGS) config_parser_test.cc config_parser.cc request_handler.cc hello_world_handler.cc echo_handler.cc static_file_handler.cc utils.cc $(GTEST_DIR)/src/gtest_main.cc libgtest.a -o config_parser_test $(LDFLAGS)

clean:
	$(RM) webserver config_parser_test *.o *~

.PHONY: all clean

-include $(SRCS:.cc=.d)