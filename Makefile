CXX = g++
CXXFLAGS = -std=c++0x -Wall -g
OBJS = webserver.o config_parser.o webserver_main.o
LDFLAGS = -lboost_system
GTEST_DIR = gtest-1.7.0
TESTFLAGS = -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) -pthread

all: webserver test
webserver: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

test: config_parser_test
	./config_parser_test

config_parser_test:
	$(CXX) $(CXXFLAGS) $(TESTFLAGS) -c $(GTEST_DIR)/src/gtest-all.cc 
	ar -rv libgtest.a gtest-all.o 
	$(CXX) $(CXXFLAGS) $(TESTFLAGS) config_parser_test.cc config_parser.cc $(GTEST_DIR)/src/gtest_main.cc libgtest.a -o config_parser_test $(LDFLAGS)

utils.o: utils.h
webserver.o: webserver.h
config_parser.o: config_parser.h
webserver_main.o: webserver_main.cc webserver.h config_parser.h utils.h
	$(CXX) $(CXXFLAGS) -c webserver_main.cc
clean:
	$(RM) webserver config_parser_test *.o *~
