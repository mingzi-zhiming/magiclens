#include "myserverthread.h"

MyServerThread::MyServerThread()
{
    startServer();
}

void MyServerThread::run()
{
    //std::cout<<"hi"<<std::endl;
    /*for(int i=0;i<16;i++)
        {
         matrix[i]=(float)i+1;
         std::cerr<<"matrix:" << matrix[i] << std::endl;
        }*/
    memcpy(buf,matrix,sizeof(float)*16);
    sendMSG();
}

void MyServerThread::startServer()
{

    //TCP socket
    /* now create the server socket
       make it an IPV4 socket (PF_INET) and stream socket (TCP)
       and 0 to select default protocol type */
    if ( (server_skt = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
        std::cerr<<"socket creation failed"<<std::endl;
        exit(1);
    }

    int enableReuse = 1;

    setsockopt( server_skt, SOL_SOCKET,  SO_REUSEADDR, &enableReuse, sizeof(enableReuse));


    //Initail, bind to port
    /* now fill in values of the server sockaddr_in struct
       s_addr and sin_port are in Network Byte Order (Big Endian)
       Since Intel CPUs use Host Byte Order (Little Endian), conversion
       is necessary (e.g. htons(), and htonl() */
    server_addr.sin_family = AF_INET;//IPv4
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(server_addr.sin_zero, 0, 8);

    //binding

     /* now bind server port
           associate socket (server) positionwith IP address:port (server_addr) */

    if ( bind(server_skt, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1 ){
        std::cerr<<"socket bind failed"<<std::endl;
        exit(1);
    }

    int queueSize=1;
    //Start listening, wait for connection from client with a pending queue of size
    if ( listen(server_skt, queueSize) == -1 ){
        std::cerr<<"socket listen failed"<<std::endl;
        exit(1);
    }

    //Wait for connect!
    bool gotClient=false;
    while(!gotClient){
        sin_size = sizeof(struct sockaddr_in);
        socklen_t temp=sin_size;
        std::cout<<"server starts"<<std::endl;
        if ( (client_skt = accept(server_skt, (struct sockaddr*)&clients_addr, &temp)) == -1 ){
            std::cerr<<"socket error"<<std::endl;
            exit(1);
        }
        else
        {
            gotClient=true;
            std::cout<<"Got client"<<std::endl;
        }
    }
}

void MyServerThread::sendMSG()
{
    if ( (numbytes = send(client_skt, buf, 16*sizeof(float) ,0)) == -1){
        std::cerr<<"server, normal send error"<<std::endl;
        exit(1);
    }
}
void MyServerThread::recvMSG()
{
    if ( (numbytes = recv(client_skt, buf, 16*sizeof(float),0)) == -1 ){
        std::cerr<<"server, normal recv error"<<std::endl;
        exit(1);
    }
}

void MyServerThread::sendMatrix(Matrixd transNode)
{
  float matrix1[16];
   
  Vec3 objTrans = transNode.getTrans();
  Quat objQuad = transNode.getRotate();
  
  //cout << "  Orientation  " << objQuad.x() << "," << objQuad.y() << "," << objQuad.z() << "," << objQuad.w() << endl; // orientation
  //cout << "  Postion  " << objTrans.x() << "," << objTrans.y() << "," << objTrans.z() << endl; // position
  
/*for (int i=0; i<4; ++i){
    for (int j=0; j<4; ++j)
       {
        cout << transNode(i,j) << "   " ; // position
       }
  cout << endl;
} */

cout << endl;

  for (int i=0; i<4; ++i)
    for (int j=0; j<4; ++j)
        matrix[i*4+j] = transNode(i,j);
        
 for(int i=0; i<16; i++)
        {
         cout <<"matrix:" << i << "   " << matrix1[i] << endl;
        }

  //memcpy(buf,matrix1,sizeof(float)*16);
  //sendMSG();
}


