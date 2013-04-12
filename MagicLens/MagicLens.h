#ifndef _MODELLOADER_
#define _MODELLOADER_

#include <cvrKernel/CVRPlugin.h>
#include <cvrKernel/FileHandler.h>
#include <cvrKernel/SceneObject.h>
#include <cvrMenu/SubMenu.h>
#include <cvrMenu/MenuButton.h>

#include <cvrConfig/ConfigManager.h>
#include <cvrKernel/SceneManager.h>
#include <cvrMenu/MenuSystem.h>
#include <cvrKernel/PluginHelper.h>
#include <cvrKernel/ComController.h>
#include <cvrKernel/InteractionManager.h>

#include <iostream>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <string>

#include <assert.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <queue>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <OpenThreads/Mutex>
#include <OpenThreads/Thread>

#include <osg/Matrix>
#include <osg/Camera>
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
#include <osg/Matrixd> 

#define PORT 28888
#define BUFFERSIZE 3000 

using namespace osg;

class MagicLens : public cvr::CVRPlugin
{
    public:        
        MagicLens();
        virtual ~MagicLens();
	bool init();
        void preFrame();
        osgViewer::Viewer viewer;



    protected:
        float matrix[48];
	float angles[3];
	float track_m[4][4];
	float pos[3];
 	float* m_rotateX;
	float* m_rotateY;
	float count;
         
	int server_skt, client_skt, numbytes;
	struct sockaddr_in server_addr;
	struct sockaddr_in clients_addr;
	int sin_size;
	char buf[BUFFERSIZE];
	void* lib_handle;

		typedef void* (*trackReader_t)(int shmKey);
		trackReader_t trackdInitTrackerReader;

		typedef void (*getPosition_t) (void* tracker, int id, float* pos);
		getPosition_t getPosition;		

		typedef void (*getEuler_t)(void* tracker, int id, float* orn);    
		getEuler_t getEulerAngles;

		typedef void (*getMatrix_t) (void* tracker, int id, float mat[4][4]);
		getMatrix_t getMatrix;

	void* tracker;

	float* identity(float* mat);
	void startServer();
	void sendMSG();
	void recvMSG();
	void sendMatrix();
};
#endif
