

#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>

#include <thread>
#include <pthread.h>

#include <unistd.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>

#include <sys/socket.h>
#include <netinet/in.h>

#include "CLI.h"

using namespace std;

// edit your ClientHandler interface here:
class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};

class SocketIO : public DefaultIO {
    int clientNum;
public:
    SocketIO(int client):clientNum(client){}
    virtual string read();
    virtual void write(string text);
    virtual void write(float f);
    virtual void read(float* f);
};


// you can add helper classes here and implement on the cpp file


// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
	public:
    virtual void handle(int clientID){

    }
};


// implement on Server.cpp
class Server {
	thread* t; // the thread to run the start() method in
    bool isStopped;
    int fileDesc;

    sockaddr_in server;
    sockaddr_in client;

	// you may add data members

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
