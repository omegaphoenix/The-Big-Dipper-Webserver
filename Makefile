CXX = g++
CXXFLAGS = -std=c++0x -Wall -g
LDFLAGS = -lboost_system
GTEST_DIR = gtest-1.7.0
TESTFLAGS = -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) -pthread
PROG = webserver config_parser_test
SRCS = webserver.cc config_parser.cc request_handler.cc hello_world_handler.cc echo_handler.cc static_file_handler.cc utils.cc

all: test webserver
webserver: $(SRCS:.cpp=.o) webserver_main.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	./webserver config_file

test: config_parser_test
	./config_parser_test

config_parser_test:
	$(CXX) $(CXXFLAGS) $(TESTFLAGS) -c $(GTEST_DIR)/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o
	$(CXX) $(CXXFLAGS) $(TESTFLAGS) $(SRCS) config_parser_test.cc $(GTEST_DIR)/src/gtest_main.cc libgtest.a -o config_parser_test $(LDFLAGS)

clean:
	$(RM) $(PROG) *.o *~

.PHONY: all clean

-include $(SRCS:.cc=.d)
