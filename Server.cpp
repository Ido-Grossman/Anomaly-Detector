
#include "Server.h"

string SocketIO::read() {
    string word = "";
    char letter = 0;
    while (letter != '\n') {
        recv(clientNum, &letter, sizeof(char), 0);
        word += letter;
    }
    return word;
}

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

void handler(int sigNum){
    cout<<"sidH"<<endl;
}

void Server::start(ClientHandler& ch)throw(const char*) {
    t = new thread([&ch, this]() {
        signal(SIGALRM, handler);
        while (!isStopped) {
            socklen_t cSize = sizeof(client);
            alarm(1);
            int acceptClient = accept(fileDesc, (struct sockaddr*)&client, &cSize);
            if(acceptClient > 0){
                //new thread([&aClient,this,&ch](){
                ch.handle(acceptClient);
                close(acceptClient);
                //});
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

