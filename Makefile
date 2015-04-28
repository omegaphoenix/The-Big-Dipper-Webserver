CXX = g++
CXXFLAGS = -std=c++0x -Wall -g
all: webserver
webserver: webserver.o config_parser.o webserver_main.o
	$(CXX) $(CXXFLAGS) -o webserver webserver.o config_parser.o webserver_main.o -lboost_system
webserver.o: webserver.h
config_parser.o: config_parser.h
webserver_main.o: webserver_main.cc webserver.h config_parser.h
	$(CXX) $(CXXFLAGS) -c webserver_main.cc
clean:
	$(RM) webserver *.o