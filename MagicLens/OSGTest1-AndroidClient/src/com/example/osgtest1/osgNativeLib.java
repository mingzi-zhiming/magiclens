package com.example.osgtest1;

public class osgNativeLib {
	static {
		System.loadLibrary("osgNativeLib");
	}
	
	public static native void OSGRun();
	public static native void initOSG(int width,int height);
	public static native void mouseMoveEvent(float x,float y);
    public static native void mouseButtonPressEvent(float x,float y, int button);
    public static native void mouseButtonReleaseEvent(float x,float y, int button);
    public static native void getServerIPandStartClient(String IP);
	public static native void sendMsg();
	public static native void recvMsg();
	public static native void connectToServer();
}
