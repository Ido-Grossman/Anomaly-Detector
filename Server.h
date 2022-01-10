// ID: 324603422 (Santiago Szterenberg) && 208985424 (Ido Grossman)

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

// The hander interface to handle the client.
class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};

// The socketIO class needed for the IO with server.
class SocketIO : public DefaultIO {
    int clientNum;
public:
    SocketIO(int client):clientNum(client){}
    virtual string read();
    virtual void write(string text);
    virtual void write(float f);
    virtual void read(float* f);
};


// The anomalyDetectionHandler handles the anomaly detection of the given client.
class AnomalyDetectionHandler:public ClientHandler{
	public:
    virtual void handle(int clientID){
        SocketIO socketio(clientID);
        CLI cli(&socketio);
        cli.start();
    }
};


// The server methods required to run the server.
class Server {
	thread* t; // The thread to run the start() method in
    bool isStopped; // Is the server stopped or not.
    int fileDesc;   // the fileDesc of the socket.

    sockaddr_in server; // The server sockaddr struct.
    sockaddr_in client; // The client sockaddr struct.

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
