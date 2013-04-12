/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_example_osgtest1_osgNativeLib */

#ifndef _Included_com_example_osgtest1_osgNativeLib
#define _Included_com_example_osgtest1_osgNativeLib
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_example_osgtest1_osgNativeLib
 * Method:    OSGRun
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_example_osgtest1_osgNativeLib_OSGRun
  (JNIEnv *, jclass);

/*
 * Class:     com_example_osgtest1_osgNativeLib
 * Method:    initOSG
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_example_osgtest1_osgNativeLib_initOSG
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     com_example_osgtest1_osgNativeLib
 * Method:    mouseMoveEvent
 * Signature: (FF)V
 */
JNIEXPORT void JNICALL Java_com_example_osgtest1_osgNativeLib_mouseMoveEvent
  (JNIEnv *, jclass, jfloat, jfloat);

/*
 * Class:     com_example_osgtest1_osgNativeLib
 * Method:    mouseButtonPressEvent
 * Signature: (FFI)V
 */
JNIEXPORT void JNICALL Java_com_example_osgtest1_osgNativeLib_mouseButtonPressEvent
  (JNIEnv *, jclass, jfloat, jfloat, jint);

/*
 * Class:     com_example_osgtest1_osgNativeLib
 * Method:    mouseButtonReleaseEvent
 * Signature: (FFI)V
 */
JNIEXPORT void JNICALL Java_com_example_osgtest1_osgNativeLib_mouseButtonReleaseEvent
  (JNIEnv *, jclass, jfloat, jfloat, jint);

/*
 * Class:     com_example_osgtest1_osgNativeLib
 * Method:    getServerIPandStartClient
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_example_osgtest1_osgNativeLib_getServerIPandStartClient
  (JNIEnv *, jclass, jstring);

/*
 * Class:     com_example_osgtest1_osgNativeLib
 * Method:    sendMsg
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_example_osgtest1_osgNativeLib_sendMsg
  (JNIEnv *, jclass);

/*
 * Class:     com_example_osgtest1_osgNativeLib
 * Method:    recvMsg
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_example_osgtest1_osgNativeLib_recvMsg
  (JNIEnv *, jclass);

/*
 * Class:     com_example_osgtest1_osgNativeLib
 * Method:    connectToServer
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_example_osgtest1_osgNativeLib_connectToServer
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif