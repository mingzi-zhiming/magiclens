package com.example.osgtest1;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView.Renderer;
import android.util.Log;

public class MyGLRenderer implements Renderer{
	public MyGLRenderer()
	{
	}
	@Override
	public void onDrawFrame(GL10 gl) {
        osgNativeLib.OSGRun();
		
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		// TODO Auto-generated method stub
		Log.i("OnSurfaceChange","in");
		osgNativeLib.initOSG(width,height);
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        gl.glEnable(GL10.GL_DEPTH_TEST);             //Enables Depth Testing
	}

}
