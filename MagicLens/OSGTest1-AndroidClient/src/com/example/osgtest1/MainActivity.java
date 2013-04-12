package com.example.osgtest1;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.graphics.PointF;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.WindowManager;
import android.widget.EditText;

import java.lang.Math;


public class MainActivity extends Activity implements OnTouchListener{
    /** Called when the activity is first created. */
    GLSurfaceView mview;
 
    private static final String TAG = "OSG Activity"; 
	String ip = new String("137.110.119.176");
	
    enum moveTypes { NONE , DRAG , ZOOM } 
    enum navType { PRINCIPAL }
       
    moveTypes mode=moveTypes.NONE;
    navType navMode = navType.PRINCIPAL;
   
    PointF oneFingerOrigin = new PointF(0,0);
    long timeOneFinger=0;
    PointF twoFingerOrigin = new PointF(0,0);
    long timeTwoFinger=0;
    float distanceOrigin;
    


    
   
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        setContentView(R.layout.activity_main);
        mview =new GLSurfaceView(this);
        mview.setRenderer(new MyGLRenderer());
        mview.setOnTouchListener(this);
        setContentView(mview);
    }
    
    //Android Life Cycle Menu
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.activity_main, menu);
        return super.onCreateOptionsMenu(menu);
    }

    /*Android Menu ...
     * Start Client --> Get the IP address from the server 
     * Connect Server --> Connect the server to accept the client
     */
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle item selection
        switch (item.getItemId()) {
                                    case R.id.menu_settings:
                                    	 //Start Client
        	                             //Server IP Address
        	                  			 osgNativeLib.getServerIPandStartClient(ip);
                                         return true;
                                         
                                    case R.id.ConnectServer:
                                    	 //Connect to the Server
                                    	 //Receive Message from the Server
        	                             osgNativeLib.connectToServer();
        	                             osgNativeLib.recvMsg();
                                         return true;
                                         
                                    case R.id.SetIP:
                                    	 AlertDialog.Builder alert = new AlertDialog.Builder(this);
                                    	  
                                    	 alert.setTitle("Set IP Address");
                                    	 alert.setMessage("Current IP Address is " + ip);
                                    
                                    	 final EditText input = new EditText(this);
                                    	 input.setInputType(3);
                                    	 alert.setView(input);
                                    	 alert.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                                    		 public void onClick(DialogInterface dialog, int whichButton) {
                                    			 String value = input.getText().toString();
                                    			 ip = value;
                                    		 }
                                    	 });
                                    	 alert.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                                    		 
											
											@Override
											public void onClick(DialogInterface dialog, int which) {
												//cancelled 
												
											}
										});
                                    	 alert.show();
                                    	 
        	                             return true;
                                    
                                         
                                    /* If you need these to function in the menu add these two lines on the activity_main.xml
                                     *     <item android:id="@+id/Send" android:title="@string/menu_settings_2"></item>
                                     *     <item android:id="@+id/Recv" android:title="@string/menu_settings_3"></item>
                                     */
                                    //case R.id.Send:
        	                             //osgNativeLib.sendMsg();
        	                             //return true;
                                    //case R.id.Recv:
        	                             //osgNativeLib.recvMsg();
                                         //return true;
                                    default:
                                         return super.onOptionsItemSelected(item);
                                 }
    }
   
    @Override
    protected void onPause()
    {
        super.onPause();
        mview.onPause();
    }
   
    @Override
    protected void onResume()
    {
        super.onResume();
        mview.onResume();
    }

    /*This part of code from the "osgAndroidExampleGLES1" it use to translate and rotate the object by touch screen with one finger 
     *we can change the code in this function osgNativeLib.mouseButtonPressEvent(event.getX(0), event.getY(0), 1);
     *  1 --> Rotate the object by touch screen
     *  2 --> Translate the object by touch screen
     */
    @Override
    public boolean onTouch(View v, MotionEvent event) {
       
        int n_points = event.getPointerCount();
        int action = event.getActionMasked();
       
        switch(n_points){
        case 1:
            switch(action){
           
             case MotionEvent.ACTION_DOWN:
                mode = moveTypes.DRAG;
               
                osgNativeLib.mouseMoveEvent(event.getX(0), event.getY(0));
                if(navMode==navType.PRINCIPAL)
                    osgNativeLib.mouseButtonPressEvent(event.getX(0), event.getY(0), 1); //2
                else
                    osgNativeLib.mouseButtonPressEvent(event.getX(0), event.getY(0), 1);
               
                oneFingerOrigin.x=event.getX(0);
                oneFingerOrigin.y=event.getY(0);
                break;
               
            /*case MotionEvent.ACTION_CANCEL:
                switch(mode){
                case DRAG:
                    osgNativeLib.mouseMoveEvent(event.getX(0), event.getY(0));
                    if(navMode==navType.PRINCIPAL)
                        osgNativeLib.mouseButtonReleaseEvent(event.getX(0), event.getY(0), 2);
                    else
                        osgNativeLib.mouseButtonReleaseEvent(event.getX(0), event.getY(0), 1);
                    break;
                default :
                    Log.e(TAG,"There has been an anomaly in touch input 1point/action");
                }
                mode = moveTypes.NONE;
                break;*/
               
            case MotionEvent.ACTION_MOVE:
               
                osgNativeLib.mouseMoveEvent(event.getX(0), event.getY(0));
               
                oneFingerOrigin.x=event.getX(0);
                oneFingerOrigin.y=event.getY(0);
               
                break;
               
            case MotionEvent.ACTION_UP:
                switch(mode){
                case DRAG:
                    if(navMode==navType.PRINCIPAL)
                        osgNativeLib.mouseButtonReleaseEvent(event.getX(0), event.getY(0), 1); //2
                    else
                        osgNativeLib.mouseButtonReleaseEvent(event.getX(0), event.getY(0), 1);
                    break;
                default :
                    Log.e(TAG,"There has been an anomaly in touch input 1 point/action");
                }
                mode = moveTypes.NONE;
                break;
               
            default :
                Log.e(TAG,"1 point Action not captured");   
            }
            break;
        
        //This case use to zoom in and zoom out the object by two finger
      
        /*case 2:
            switch (action){
           
            case MotionEvent.ACTION_POINTER_DOWN:
                //Free previous Action
                switch(mode){
                             case DRAG:
                             if(navMode==navType.PRINCIPAL)
                                  osgNativeLib.mouseButtonReleaseEvent(event.getX(0), event.getY(0), 2);
                             else
                                osgNativeLib.mouseButtonReleaseEvent(event.getX(0), event.getY(0), 1);
                             break;
                            }
               
                mode = moveTypes.ZOOM;
                distanceOrigin = sqrDistance(event);
                twoFingerOrigin.x=event.getX(1);
                twoFingerOrigin.y=event.getY(1);
                oneFingerOrigin.x=event.getX(0);
                oneFingerOrigin.y=event.getY(0);
               
                osgNativeLib.mouseMoveEvent(oneFingerOrigin.x,oneFingerOrigin.y);
                osgNativeLib.mouseButtonPressEvent(oneFingerOrigin.x,oneFingerOrigin.y, 3);
                //osgNativeLib.mouseMoveEvent(oneFingerOrigin.x,oneFingerOrigin.y);
               
            case MotionEvent.ACTION_MOVE:
                float distance = sqrDistance(event);
                float result = distance-distanceOrigin;
                distanceOrigin=distance;
               
                if(result>1||result<-1){
                    oneFingerOrigin.y=oneFingerOrigin.y+result;
                    osgNativeLib.mouseMoveEvent(oneFingerOrigin.x,oneFingerOrigin.y);
                }
               
                break;
               
            case MotionEvent.ACTION_POINTER_UP:
                mode =moveTypes.NONE; 
                osgNativeLib.mouseButtonReleaseEvent(oneFingerOrigin.x,oneFingerOrigin.y, 3);
                break;
               
            case MotionEvent.ACTION_UP:
                mode =moveTypes.NONE;
                osgNativeLib.mouseButtonReleaseEvent(oneFingerOrigin.x,oneFingerOrigin.y, 3);
                break;
               
            default :
                Log.e(TAG,"2 point Action not captured");
            }
            break;  */        
        }
           
        return true;
    }
    
}
