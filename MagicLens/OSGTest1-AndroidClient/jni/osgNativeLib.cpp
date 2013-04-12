#include "com_example_osgtest1_osgNativeLib.h"
#include "osgMain.h"
osgMain osgmain;

JNIEXPORT void JNICALL Java_com_example_osgtest1_osgNativeLib_OSGRun
  (JNIEnv *, jclass)
{
	osgmain.draw();
}


JNIEXPORT void JNICALL Java_com_example_osgtest1_osgNativeLib_initOSG
  (JNIEnv *, jclass, jint width, jint height)
{
	osgmain.initOSG(width, height);
}

JNIEXPORT void JNICALL Java_com_example_osgtest1_osgNativeLib_mouseMoveEvent
  (JNIEnv *, jclass, jfloat x, jfloat y)
{
	osgmain.mouseMoveEvent(x,y);
}

JNIEXPORT void JNICALL Java_com_example_osgtest1_osgNativeLib_mouseButtonPressEvent
  (JNIEnv *, jclass, jfloat x, jfloat y, jint button){
	osgmain.mouseButtonPressEvent(x,y,button);
}
JNIEXPORT void JNICALL Java_com_example_osgtest1_osgNativeLib_mouseButtonReleaseEvent
  (JNIEnv *, jclass, jfloat x, jfloat y, jint button){
	osgmain.mouseButtonReleaseEvent(x,y,button);
}

JNIEXPORT void JNICALL Java_com_example_osgtest1_osgNativeLib_getServerIPandStartClient
  (JNIEnv *env, jclass, jstring jserverIP)
{
	const char * IPtemp=env->GetStringUTFChars(jserverIP,0);
	osgmain.serverIPlength=env->GetStringLength(jserverIP);
	free(osgmain.serverIP);
	osgmain.serverIP=new char[osgmain.serverIPlength];
	strcpy(osgmain.serverIP,IPtemp);
	__android_log_print(ANDROID_LOG_INFO,"jni client","Server IP:%s",osgmain.serverIP );
	//env->ReleaseStringUTFChars(jserverIP,IPtemp);
	osgmain.startClient();
}

JNIEXPORT void JNICALL Java_com_example_osgtest1_osgNativeLib_sendMsg
  (JNIEnv *, jclass)
{
	osgmain.sendMSG();
}


JNIEXPORT void JNICALL Java_com_example_osgtest1_osgNativeLib_recvMsg
  (JNIEnv *, jclass)
{
	osgmain.recvMSG();
}


JNIEXPORT void JNICALL Java_com_example_osgtest1_osgNativeLib_connectToServer
  (JNIEnv *, jclass)
{
	osgmain.connectServer();
}
