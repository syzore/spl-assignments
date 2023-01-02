CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system -lpthread

all: clean StompWCIClient

StompClient: bin/ConnectionHandler.o bin/stompClient.o
	g++ -o bin/StompClient bin/ConnectionHandler.o bin/stompClient.o $(LDFLAGS)

StompWCIClient: bin/ConnectionHandler.o bin/StompClient.o bin/event.o bin/stringUtil.o
	g++ -o bin/StompWCIClient bin/ConnectionHandler.o bin/StompClient.o bin/Event.o bin/StringUtil.o $(LDFLAGS)

bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	g++ $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/stompClient.o: src/stompClient.cpp
	g++ $(CFLAGS) -o bin/stompClient.o src/stompClient.cpp

bin/event.o: src/event.cpp
	g++ $(CFLAGS) -o bin/event.o src/event.cpp

bin/stringUtil.o: src/stringUtil.cpp
	g++ $(CFLAGS) -o bin/stringUtil.o src/stringUtil.cpp

bin/StompClient.o: src/StompClient.cpp
	g++ $(CFLAGS) -o bin/StompClient.o src/StompClient.cpp

.PHONY: clean
clean:
	rm -f bin/*
	