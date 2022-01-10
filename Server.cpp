// ID: 324603422 (Santiago Szterenberg) && 208985424 (Ido Grossman)
#include "Server.h"

// this function reads from the client's buffer
string SocketIO::read() {
    string word = "";
    char letter = 0;
    while (letter != '\n') {
        recv(clientNum, &letter, sizeof(char), 0);
        word += letter;
    }
    return word;
}

// this function writes through stream
void SocketIO::write(float f) {
    ostringstream stream;
    stream << f;
    write(string(stream.str()));
}

void SocketIO::write(string text) {
    const char* word = text.c_str();
    send(clientNum, word, strlen(word), 0);
}

void SocketIO::read(float *f) {

}

// this function initialize the tcp socket
Server::Server(int port)throw (const char*) {
    isStopped = false;
    fileDesc = socket(AF_INET, SOCK_STREAM, 0);
    if (fileDesc < 0)
        throw "socket failed";
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if(bind(fileDesc,(struct sockaddr*)&server, sizeof(server))<0)
        throw "bind failure";

    if(listen(fileDesc, 3)<0)
        throw "listen failure";
}

// when we get an alarm we write to the console
void handler(int sigNum){
    cout<<"sidH"<<endl;
}

// this function starts the server and accepts the clients one by one
void Server::start(ClientHandler& ch)throw(const char*) {
    t = new thread([&ch, this]() {
        signal(SIGALRM, handler);
        while (!isStopped) {
            socklen_t cSize = sizeof(client);
            alarm(1);
            int acceptClient = accept(fileDesc, (struct sockaddr*)&client, &cSize);
            if(acceptClient > 0){
                ch.handle(acceptClient);
                close(acceptClient);
            }
            alarm(0);

        }
        close(fileDesc);
    });
}

void Server::stop(){
    isStopped = true;
	t->join(); // do not delete this!
}

Server::~Server() = default;

