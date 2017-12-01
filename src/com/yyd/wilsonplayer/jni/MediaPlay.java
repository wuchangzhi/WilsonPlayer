package com.yyd.wilsonplayer.jni;

import android.view.Surface;

public class MediaPlay {
	static{
		System.loadLibrary("mediaplay");
	}
	public native static void play(String filename, Object surface);
	public native static void test(String filename);
}
