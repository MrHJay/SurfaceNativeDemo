package com.zqautomotive.surfacenativedemo;

import android.content.res.AssetManager;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.Display;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.WindowManager;

import java.io.IOException;

public class MainActivity extends AppCompatActivity implements SurfaceHolder.Callback, SurfaceTexture.OnFrameAvailableListener {

    static final String TAG = "MainActivity";

    private static final int VIDEO_WIDTH = 1280;  // dimensions for 720p video
    private static final int VIDEO_HEIGHT = 720;
    private static final int DESIRED_PREVIEW_FPS = 15;

    private Camera mCamera;
    private SurfaceView surfaceView;
    private SurfaceTexture mSurfaceTexture;
    private int mCameraPreviewThousandFps;
    private int mTextureId;
    private SurfaceHolder mSurfaceHolder;
    private boolean hasSurface;
    private GestureDetector gestureDetector;
    private GestureDetector.SimpleOnGestureListener listener = new GestureDetector.SimpleOnGestureListener() {
        float mScale = 1;
        private static final float step = 1.5f;
        private static final float maxScale = step * step * step;

        @Override
        public boolean onDoubleTap(MotionEvent e) {
            mScale *= step;
            if (mScale > maxScale) {
                mScale = 1;
            }
            zoom(e.getX(), e.getY(), mScale);
            return super.onDoubleTap(e);
        }

        @Override
        public void onLongPress(MotionEvent e) {
            reset();
            super.onLongPress(e);
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        surfaceView = findViewById(R.id.surfaceView);
        mSurfaceHolder = surfaceView.getHolder();
        gestureDetector = new GestureDetector(this, listener);
    }

    @Override
    protected void onStart() {
        super.onStart();
        Log.i(TAG, "onStart: ");
        if (!hasSurface) {
            mSurfaceHolder.addCallback(this);
        }

    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.i(TAG, "onResume: ");
        if (hasSurface) {
            openCamera(VIDEO_WIDTH, VIDEO_HEIGHT, DESIRED_PREVIEW_FPS);
            startPreview(mSurfaceHolder);
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        Log.i(TAG, "onPause: ");
        if (mCamera != null) {
            mCamera.stopPreview();
            mCamera.release();
            mCamera = null;
        }

    }

    @Override
    protected void onStop() {
        super.onStop();
        Log.i(TAG, "onStop: ");
        if (!hasSurface) {
            mSurfaceHolder.removeCallback(this);
        }
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        return gestureDetector.onTouchEvent(event) || super.onTouchEvent(event);
    }

    private void openCamera(int desiredWidth, int desiredHeight, int desiredFps) {
        if (mCamera != null) {
            throw new RuntimeException("camera already initialized");
        }

        Camera.CameraInfo info = new Camera.CameraInfo();

        // Try to find a front-facing camera (e.g. for videoconferencing).
        int numCameras = Camera.getNumberOfCameras();
        for (int i = 0; i < numCameras; i++) {
            Camera.getCameraInfo(i, info);
            if (info.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
                mCamera = Camera.open(i);
                break;
            }
        }
        if (mCamera == null) {
            Log.d(TAG, "No front-facing camera found; opening default");
            mCamera = Camera.open();    // opens first back-facing camera
        }
        if (mCamera == null) {
            throw new RuntimeException("Unable to open camera");
        }

        Camera.Parameters parms = mCamera.getParameters();

        CameraUtils.choosePreviewSize(parms, desiredWidth, desiredHeight);

        // Try to set the frame rate to a constant value.
        mCameraPreviewThousandFps = CameraUtils.chooseFixedPreviewFps(parms, desiredFps * 1000);

        // Give the camera a hint that we're recording video.  This can have a big
        // impact on frame rate.
        parms.setRecordingHint(true);

        mCamera.setParameters(parms);

        Camera.Size cameraPreviewSize = parms.getPreviewSize();
        String previewFacts = cameraPreviewSize.width + "x" + cameraPreviewSize.height +
                " @" + (mCameraPreviewThousandFps / 1000.0f) + "fps";
        Log.i(TAG, "Camera config: " + previewFacts);

        Display display = ((WindowManager) getSystemService(WINDOW_SERVICE)).getDefaultDisplay();

        if (display.getRotation() == Surface.ROTATION_0) {
            mCamera.setDisplayOrientation(90);
        } else if (display.getRotation() == Surface.ROTATION_270) {
            mCamera.setDisplayOrientation(180);
        } else {
            // Set the preview aspect ratio.
            // layout.setAspectRatio((double) cameraPreviewSize.width / cameraPreviewSize.height);
        }
    }

    private void startPreview(SurfaceHolder surfaceHolder) {
        if (mCamera != null) {
            Log.d(TAG, "starting camera preview");
            try {
                mCamera.setPreviewTexture(mSurfaceTexture);
//                mCamera.setPreviewDisplay(surfaceHolder);
            } catch (IOException ioe) {
                throw new RuntimeException(ioe);
            }
            mCamera.startPreview();
        }
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Log.i(TAG, "surfaceCreated: ");
        if (!hasSurface) {
            openCamera(VIDEO_WIDTH, VIDEO_HEIGHT, DESIRED_PREVIEW_FPS);

            nativeSurfaceCreated(holder.getSurface(), getAssets());
            mTextureId = nativeCreateTexture();
            mSurfaceTexture = new SurfaceTexture(mTextureId);
            mSurfaceTexture.setOnFrameAvailableListener(this);

            startPreview(holder);
            hasSurface = true;
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        Log.i(TAG, "surfaceChanged: ");
        nativeSurfaceChanged(width, height);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.i(TAG, "surfaceDestroyed: ");
        nativeSurfaceDestroyed(holder.getSurface());

        if (mSurfaceTexture != null) {
            mSurfaceTexture.release();
            mSurfaceTexture = null;
        }

        hasSurface = false;
    }

    private float[] tmpMatrix = new float[16];

    @Override
    public void onFrameAvailable(SurfaceTexture surfaceTexture) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (mSurfaceTexture != null) {
                    mSurfaceTexture.updateTexImage();
                    long timestamp = mSurfaceTexture.getTimestamp();
                    mSurfaceTexture.getTransformMatrix(tmpMatrix);
                    drawFrame(tmpMatrix, System.currentTimeMillis());
                }
            }
        });
    }

    public native void changeColor();

    public native void nativeSurfaceCreated(Surface surface, AssetManager assetManager);

    public native void nativeSurfaceChanged(int width, int height);

    public native void nativeSurfaceDestroyed(Surface surface);

    private native int nativeCreateTexture();

    private native void drawFrame(float[] matrix, long timestamp);

    private native void zoom(float x, float y, float scale);

    private native void reset();
}
