#!/bin/bash

case `uname` in
  Linux) g++ webserver.cc webserver_main.cc config_parser.cc -std=c++0x -g -Wall -o webserver -lboost_system;;
  Darwin) clang++ webserver.cc webserver_main.cc config_parser.cc -std=c++11 -g -Wall -stdlib=libc++ -o webserver -lboost_system;;
  *) echo "Unknown operating system";;
esac
