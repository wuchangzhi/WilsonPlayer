package com.yyd.wilsonplayer.widget;

import android.content.Context;
import android.graphics.PixelFormat;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.SurfaceView;

public class VideoView extends SurfaceView implements Callback {
	public VideoView(Context context) {
		this(context,null);
	}

	public VideoView(Context context, AttributeSet attrs) {
		this(context, attrs,0);
	}

	public VideoView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		init();
	}

	

	private void init() {
		getHolder().addCallback(this);
		getHolder().setFormat(PixelFormat.RGBA_8888);
	}

	@Override
	public void surfaceCreated(SurfaceHolder holder) {
				
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
				
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
				
	}
	
}
