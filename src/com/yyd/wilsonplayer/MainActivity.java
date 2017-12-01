package com.yyd.wilsonplayer;

import java.io.File;

import com.yyd.wilsonplayer.jni.MediaPlay;
import com.yyd.wilsonplayer.widget.VideoView;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;

public class MainActivity extends Activity implements OnClickListener {

	private EditText mFilename;
	private Button mPlay;
	private VideoView mVideoView;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		initView();
	}

	private void initView() {
		mFilename = (EditText) findViewById(R.id.filename);
		mPlay = (Button) findViewById(R.id.play);
		mVideoView = (VideoView) findViewById(R.id.video_view);
		
		mPlay.setOnClickListener(this);
	}

	@Override
	public void onClick(View v) {
		final String filename = Environment.getExternalStorageDirectory() + File.separator + mFilename.getText().toString();
		Log.d("testonClick", filename);
		if(!TextUtils.isEmpty(filename)){
			new Thread(new Runnable() {
				
				@Override
				public void run() {
					MediaPlay.play(filename,mVideoView.getHolder().getSurface());
				}
			}).start();
			
			//MediaPlay.test(filename);
		}
	}

}
