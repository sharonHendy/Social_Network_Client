CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-L /usr/lib/ -lboost_system -lboost_thread -lpthread

all: EchoClient
	g++ -o bin/BGSclient bin/connectionHandler.o bin/echoClient.o bin/task.o bin/encoderDecoder.o bin/logoutStatus.o $(LDFLAGS)

EchoClient: bin/connectionHandler.o bin/task.o bin/echoClient.o bin/encoderDecoder.o bin/logoutStatus.o

bin/task.o:  src/task.cpp bin/logoutStatus.o bin/encoderDecoder.o
	g++ $(CFLAGS) -o bin/task.o src/task.cpp

bin/connectionHandler.o: src/connectionHandler.cpp
	g++ $(CFLAGS) -o bin/connectionHandler.o src/connectionHandler.cpp

bin/echoClient.o: src/echoClient.cpp bin/logoutStatus.o bin/task.o bin/connectionHandler.o bin/encoderDecoder.o
	g++ $(CFLAGS) -o bin/echoClient.o src/echoClient.cpp

bin/encoderDecoder.o:  src/encoderDecoder.cpp bin/connectionHandler.o
	g++ $(CFLAGS) -o bin/encoderDecoder.o src/encoderDecoder.cpp

bin/logoutStatus.o: src/logoutStatus.cpp
	g++ $(CFLAGS) -o bin/logoutStatus.o src/logoutStatus.cpp

.PHONY: clean
clean:
	rm -f bin/*
