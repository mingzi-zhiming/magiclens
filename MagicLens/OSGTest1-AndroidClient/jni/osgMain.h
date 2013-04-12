#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <queue>
#include <OpenThreads/Mutex>
#include <OpenThreads/Thread>

#include <osg/GL>
#include <osg/GLExtensions>
#include <osg/Depth>
#include <osg/Program>
#include <osg/Shader>
#include <osg/Node>
#include <osg/Notify>
#include <osg/Geometry>
#include <osg/ShapeDrawable>
#include <osg/Material>

//osgText
#include <osgText/Text>
//osgDB
#include <osgDB/DatabasePager>
#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
//osg_viewer
#include <osgViewer/Viewer>
#include <osgViewer/Renderer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/Vec3f>
#include <osg/Quat>
#include <osg/MatrixTransform>
//osgGA
#include <osgGA/GUIEventAdapter>
#include <osgGA/MultiTouchTrackballManipulator>
#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>
#include <osgGA/SphericalManipulator>

#include <android/log.h>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <android/log.h>
#include <fcntl.h>

#define PORT 28888
#define bufferSize 2048 
#define MatrixSize 48*4

//just copied from the OSG example
//Static plugins Macro
USE_OSGPLUGIN(ive)
USE_OSGPLUGIN(osg)
USE_OSGPLUGIN(osg2)
USE_OSGPLUGIN(terrain)
USE_OSGPLUGIN(rgb)
USE_OSGPLUGIN(OpenFlight)
USE_OSGPLUGIN(dds)
//Static DOTOSG
USE_DOTOSGWRAPPER_LIBRARY(osg)
USE_DOTOSGWRAPPER_LIBRARY(osgFX)
USE_DOTOSGWRAPPER_LIBRARY(osgParticle)
USE_DOTOSGWRAPPER_LIBRARY(osgTerrain)
USE_DOTOSGWRAPPER_LIBRARY(osgText)
USE_DOTOSGWRAPPER_LIBRARY(osgViewer)
USE_DOTOSGWRAPPER_LIBRARY(osgVolume)
//Static serializer
USE_SERIALIZER_WRAPPER_LIBRARY(osg)
USE_SERIALIZER_WRAPPER_LIBRARY(osgAnimation)
USE_SERIALIZER_WRAPPER_LIBRARY(osgFX)
USE_SERIALIZER_WRAPPER_LIBRARY(osgManipulator)
USE_SERIALIZER_WRAPPER_LIBRARY(osgParticle)
USE_SERIALIZER_WRAPPER_LIBRARY(osgTerrain)
USE_SERIALIZER_WRAPPER_LIBRARY(osgText)
USE_SERIALIZER_WRAPPER_LIBRARY(osgVolume)

//pretty much the same as the OSG example
class osgMain{
private:
    osgViewer::Viewer viewer;
    osg::ref_ptr<osg::Group> root;
    osg::ref_ptr<osg::StateSet> state;
	osg::ref_ptr<osg::Node> model2;
    osg::ref_ptr<osg::MatrixTransform> mt;
    osg::ref_ptr<osg::MatrixTransform> mTransform;
    osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> _manipulator;

    bool initialized;

public:
    int client_skt;  /* client socket */
    int rc;
    struct sockaddr_in local_addr, serv_addr;
    char * serverIP;
    int serverIPlength;
    int numbytes;
    char buf[bufferSize];

    float matrix[16]; //Transform matrix
    bool firstRead;

    void draw();
    void initOSG(int width,int height);
    void mouseMoveEvent(float x, float y);
    void mouseButtonPressEvent(float x,float y,int button);
    void mouseButtonReleaseEvent(float x,float y,int button);
    void startClient();
    void connectServer();
    void sendMSG();
    void recvMSG();

    osgMain();
    ~osgMain();

};

void itoa(int val,std::string& buf,int base);
