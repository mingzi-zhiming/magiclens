//#include "myserverthread.h"
#include "MagicLens.h"

#include <cvrConfig/ConfigManager.h>
#include <cvrKernel/SceneManager.h>
#include <cvrMenu/MenuSystem.h>
#include <cvrKernel/PluginHelper.h>

#include <iostream>
#include <string.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <OpenThreads/Thread>
#include <math.h>
#include <dlfcn.h>

#include <osg/Matrix>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/CullFace>
#include <osgDB/ReadFile>
#include <osg/io_utils>
#include <osgViewer/Viewer>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <osg/Switch>
#include <osg/PolygonMode>
#include <osg/Group>

using namespace osg;
using namespace std;
using namespace cvr;

using std::cin;
using std::cout;
using std::endl;

CVRPLUGIN(MagicLens)


MagicLens::MagicLens()
{
}


MagicLens::~MagicLens()
{
}


bool MagicLens::init()
{
    
    std::cerr << "MagicLens init\n";
    count = 0; 
    // LOADING THE DYNAMIC LIBRARY
    //
    // lib_handle holds the information for the library.  
    // Always use the absolute path
    lib_handle = dlopen("/home/mmchao/trackdAPI/lib64/linux_x86_64/libtrackdAPI_c.so", RTLD_LAZY);

    // If the handle received complications, dlerror is set to an error value.  
  	 // Print out the error to see what it is.
  	 if(!lib_handle) 
	 { 
    		std::cerr << "Library failed to load." << dlerror() << endl;;
    	 }
  	else cout << "Library loaded." << endl;
	char *dlsym_error = dlerror();

	 // LOADING trackd api functions

// Storing the function trackdInitTrackerReader to be used later
	 trackdInitTrackerReader = (trackReader_t) dlsym(lib_handle, "trackdInitTrackerReader");
	// If trackdInitTrackerReader doesn't exist in the dynamic library,
    	
	// print out an error and call it quits.
    	dlsym_error = dlerror();
    	if(dlsym_error)
    	{
    		cerr << "CANNOT LOAD SYMBOL 'trackdInitTrackerReader' " << dlsym_error << endl;
    		dlclose(lib_handle);
    		return 1;
  	 }
  	 else cout << "Symbol 'trackdInitTrackerReader' loaded." << endl;

// Storing the function trackdGetEulerAngles to be used later
	 getEulerAngles = (getEuler_t) dlsym(lib_handle, "trackdGetEulerAngles");

  	 // If trackdGetEulerAngles doesn't exist in the dynamic library,
  	 // print out an error and call it quits.
   	 dlsym_error = dlerror();
    	if(dlsym_error)
	 {
    		cerr << "CANNOT LOAD SYMBOL 'GetEulerAngles' " << dlsym_error << endl;
    		dlclose(lib_handle);
    		return 1;
  	 }
  	 else cout << "Symbol 'GetEulerAngles' loaded." << endl;

// Storing the function trackdGetMatrix to be used later
	 getMatrix = (getMatrix_t) dlsym(lib_handle, "trackdGetMatrix");

  	 // If trackdGetMatrix doesn't exist in the dynamic library,
  	 // print out an error and call it quits.
    	dlsym_error = dlerror();
    	if(dlsym_error)
	 {
    		cerr << "CANNOT LOAD SYMBOL 'trackdGetMatrix' " << dlsym_error << endl;
    		dlclose(lib_handle);
    		return 1;
  	 }
  	 else cout << "Symbol 'trackdGetMatrix' loaded." << endl;

// Storing the function trackdGetPosition to be used later
	 getPosition = (getPosition_t) dlsym(lib_handle, "trackdGetPosition");

  	 // If trackdGetPosition doesn't exist in the dynamic library,
  	 // print out an error and call it quits.
    	dlsym_error = dlerror();
    	if(dlsym_error)
	 {
    		cerr << "CANNOT LOAD SYMBOL 'trackdGetPosition' " << dlsym_error << endl;
    		dlclose(lib_handle);
    		return 1;
  	 }
  	 else cout << "Symbol 'trackdGetPosition' loaded." << endl;
	
	tracker = trackdInitTrackerReader(4126);

   cout<<"==========================================="<<endl;

    bool status = false;

    //Use it with the CalVR nodes

       if(ComController::instance()->isMaster())
         {
          status = true;
          ComController::instance()->sendSlaves((char *)&status, sizeof(bool));
         }

       else
         {
          ComController::instance()->readMaster((char *)&status, sizeof(bool));
         } 


    //Load osg Object
      osg::ref_ptr<osg::Node> model1 = osgDB::readNodeFile( "cessna.osg" );  
    //You can use other osg object for example cow.osg
    

     
    //Add the osg Object to the group
    viewer.setSceneData(osgDB::readNodeFile("cessna.osg"));

     osg::ref_ptr<osg::Group> group = new osg::Group;
     group->addChild( model1.get() );

    //Add the group to the Viewer
    //viewer.setSceneData( group.get() );
    SceneManager::instance()->getObjectsRoot()->addChild(group);

    std::cerr<<"MagicLens plugin done"<<endl;

    /* There is file called MyServerThread.cpp contains only the functions that help to connect the server with the client 
       such as startServer(), sendMSG() and recvMSG(), we can use this two lines to call these functions

      MyServerThread* th=new MyServerThread();
      th->start(); */
        
    //Call start server function
    startServer();
    
    return status;
}



// Start Server function to connect to the Client
void MagicLens::startServer()
{

    //TCP socket
    /* now create the server socket
       make it an IPV4 socket (PF_INET) and stream socket (TCP)
       and 0 to select default protocol type */
    if ( (server_skt = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
       {
         std::cerr<<"socket creation failed"<<std::endl;
         exit(1);
       }

    //Disable the Socket (Change the socket settings)
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

    //Binding
    /* Now bind server port
       associate socket (server) position with IP address:port (server_addr) */

    if ( bind(server_skt, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1 )
       {
         std::cerr<<"socket bind failed"<<std::endl;
         exit(1);
       }

    
    //Start listening, wait for connection from client with a pending queue of size
    int queueSize = 2;
    if ( listen(server_skt, queueSize) == -1 )
       {
         std::cerr<<"socket listen failed"<<std::endl;
         exit(1);
       }

    //Wait for connect!
    bool gotClient=false;
    while(!gotClient)
         {
           sin_size = sizeof(struct sockaddr_in);
           socklen_t temp=sin_size;
           std::cout<<"server starts"<<std::endl;
 
           if ( (client_skt = accept(server_skt, (struct sockaddr*)&clients_addr, &temp)) == -1 )
              {
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


//Send Message for the client
void MagicLens::sendMSG()
{
    if ( (numbytes = send(client_skt, buf, 32*sizeof(float) ,0)) == -1)
       {
          if(EMSGSIZE)
			 { std::cerr<<"There is an error in Message Size" << std::endl; }
			 else{
          std::cerr<<"Server, normal send error"<<std::endl;
          }exit(1);
       }
}


//Receive Message for the client
void MagicLens::recvMSG()
{
	if ( (numbytes = recv(client_skt, buf, BUFFERSIZE,0)) == -1 )
       {
          std::cerr<<"Server, normal recv error"<<std::endl;
         
	 exit(1);
       }
}

float* MagicLens::identity(float mat[])
{
   float *mret = new float[16];
	for(int i = 0; i < 16; i++) mret[i] = 0;
   for(int i = 0; i < 16; i += 5) mret[i] = 1;
   return mret;
}

//Send the object matrix position
void MagicLens::sendMatrix()
{
   //Get object matrix position 
   Matrixd transNode = PluginHelper::getObjectMatrix();

   //Stor the elements matrix in array of 16 elements to send the array over the socket 
   for (int i=0; i<4; ++i)
       {
         for (int j=0; j<4; ++j)
			{
              matrix[i*4+j] = transNode(i,j);
			}
       }
 
	//TO DO - Caculate angles or find lib function to do
	
   
   if(tracker == NULL)
  		cerr<<"trackdInitTrackerReader is NULL"<<endl;
   else
		cerr<<"trackdInitTrackerReader working"<<endl;

    /*
    getMatrix(tracker,2,track_m);
   
    for(int i=0; i<4; i++)
    {
	for(int j=0; j<4; j++)
	{
		matrix[i*4 + j + 16] = track_m[i][j];
	}
     }
    */
	
    getPosition(tracker,2,pos);

    for(int i=0; i<3; i++)
    {
	matrix[i+16] = pos[i];
    }

    //Print the matrix 
   for(int i=16; i<19; i++)
    {
      cerr <<"matrix:" << i << "   " << matrix[i] << endl;
    }
    //osg::ref_ptr<osg::Camera>    globalCamera; 
    /*globalCamera->setViewMatrixAsLookAt(osg::Vec3d(0.0, 1.0, 0.0), 
				  osg::Vec3d(0.0, 1.0, 0.0), 
				  osg::Vec3d(0.0, 0.0, 1.0)); */
    /*   viewer.getCamera()->setViewMatrixAsLookAt( 
			osg::Vec3d(pos[0], pos[1]-30, pos[2]), 
			osg::Vec3d(20.0, 0.0, 0.0),
			osg::Vec3d(0.0, 0.0, 1.0));
    */
   
     //viewer.frame();    

    //Copy the matrix to the memory "buf"
    memcpy(buf,matrix,sizeof(float)*32);

    //Send the matrix to the client
    sendMSG();

}


//Each frame call the send matrix function to Send the object matrix position
void MagicLens::preFrame()
{
   //Updates every 5 frames to prevent crashing from data overload
   if(count == 10)
   {	//Call the send matrix function
   	sendMatrix(); 
      	count = 0;
   }
   else count++;
}


