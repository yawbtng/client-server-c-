CXX = g++
CXXFLAGS = -Wall -std=c++11

# Targets
all: server client

# Server executable
server: server.cpp utils.cpp utils.h
	$(CXX) $(CXXFLAGS) -o server server.cpp utils.cpp

# Client executable
client: client.cpp
	$(CXX) $(CXXFLAGS) -o client client.cpp

# Clean target
clean:
	rm -f server client

.PHONY: all clean

