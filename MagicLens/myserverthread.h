#ifndef MYSERVERTHREAD_H
#define MYSERVERTHREAD_H
#include <OpenThreads/Thread>
#include <stdio.h>

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <iostream>
#include <sys/param.h>
#include <osg/MatrixTransform>
#include <osg/Matrixd> 

using namespace osg;
using namespace std;

using std::cin;
using std::cout;
using std::endl;


#define PORT 28888
#define bufferSize 1024
class MyServerThread: public OpenThreads::Thread
{
public:
    int server_skt, client_skt, numbytes;
    struct sockaddr_in server_addr;
    struct sockaddr_in clients_addr;
    int sin_size;
    char buf[bufferSize];
    char trackBuf[bufferSize];
    float position[3];
    bool firstRead;
    void startServer();
    void sendMSG();
    void recvMSG();
    void sendMatrix(Matrixd transNode);

    MyServerThread();
    virtual void run();
};

#endif // MYSERVERTHREAD_H
