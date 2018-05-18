package com.zqautomotive.surfacenativedemo;

import android.app.Application;

/**
 * <p>
 * Created by jay on 18-5-7
 */
public class SurfaceApp extends Application {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    public void onCreate() {
        super.onCreate();
        initNativeEngine();
    }

    public native void initNativeEngine();
}
