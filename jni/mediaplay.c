#include <jni.h>
#include "com_yyd_wilsonplayer_jni_MediaPlay.h"
#include "android/log.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libavformat/avformat.h"

#include <android/native_window_jni.h>
#include <android/native_window.h>

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,"Wilson" ,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,"Wilson" ,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,"Wilson" ,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"Wilson" ,__VA_ARGS__)
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,"Wilson" ,__VA_ARGS__)

JNIEXPORT void JNICALL Java_com_yyd_wilsonplayer_jni_MediaPlay_test
  (JNIEnv *env, jclass jcls, jstring jpath){
	const char *c_path = (*env)->GetStringUTFChars(env,jpath,JNI_FALSE);
	LOGD("path : %s",c_path);
	(*env)->ReleaseStringUTFChars(env,jpath,c_path);
}

JNIEXPORT void JNICALL Java_com_yyd_wilsonplayer_jni_MediaPlay_play
  (JNIEnv *env, jclass jcls, jstring filename,jobject surface){
	const char *c_path = (*env)->GetStringUTFChars(env,filename,JNI_FALSE);
	LOGD("path : %s",c_path);
	AVFormatContext *avFormatCtx = avformat_alloc_context();

	//注册所有组件
	av_register_all();
	//打开视频文件
	if(avformat_open_input(&avFormatCtx,c_path,NULL,NULL)){
		LOGD("%s","avformat_open_input failed");
		return;
	}
	//获取视频信息
	if(avformat_find_stream_info(avFormatCtx,NULL) < 0){
		LOGD("%s","avformat_find_stream_info failed");
		return;
	}
	//查找视频流
	int video_stream_idx = -1;
	int i = 0;
	for(;i < avFormatCtx->nb_streams; i++){
		if(avFormatCtx->streams[i]->codec->coder_type == AVMEDIA_TYPE_VIDEO){
			video_stream_idx = i;
			break;
		}
	}
	//获取视频解码器上下文
	AVCodecContext *pCodeCtx = avFormatCtx->streams[video_stream_idx]->codec;
	//pCodeCtx->codec
	AVCodec *avcodec = avcodec_find_decoder(pCodeCtx->codec_id);
	if(avcodec == NULL){
		LOGE("%s","avcodec_find_decoder failed");
		return;
	}

	//5.打开解码器
	if(avcodec_open2(pCodeCtx,avcodec,NULL)){
		LOGE("%s","open decoder failed");
		return;
	}

    // 获取native window
    ANativeWindow* nativeWindow = ANativeWindow_fromSurface(env, surface);
	// 设置native window的buffer大小,可自动拉伸
	ANativeWindow_setBuffersGeometry(nativeWindow,  pCodeCtx->width, pCodeCtx->height, WINDOW_FORMAT_RGBA_8888);
	ANativeWindow_Buffer windowBuffer;

	AVPacket *av_pkg = av_malloc(sizeof(AVPacket));
	AVFrame *av_frame = av_frame_alloc();
	AVFrame *av_frame_rgb = av_frame_alloc();

	struct SwsContext *swsCtx = sws_getContext(pCodeCtx->width,pCodeCtx->height,pCodeCtx->pix_fmt,pCodeCtx->width,
			pCodeCtx->height,AV_PIX_FMT_RGBA,SWS_BICUBIC,NULL,NULL,NULL);
	int got_picture_ptr,framecount = 0;
	while(av_read_frame(avFormatCtx,av_pkg) >=0 ){
		int len = avcodec_decode_video2(pCodeCtx,av_frame,&got_picture_ptr,av_pkg);
		//LOGD("got_picture_ptr %d ",got_picture_ptr);
		if(got_picture_ptr) {
			LOGD("decode %d frame ",framecount++);
			ANativeWindow_lock(nativeWindow, &windowBuffer, 0);
			avpicture_fill(av_frame_rgb,windowBuffer.bits,PIX_FMT_RGB32, pCodeCtx->width, pCodeCtx->height);

			sws_scale(swsCtx,av_frame->data,av_frame->linesize,0, pCodeCtx->height,av_frame_rgb->data, av_frame_rgb->linesize);
			ANativeWindow_unlockAndPost(nativeWindow);
			usleep(1000 * 16);
		}

	}

	av_frame_free(av_frame_rgb);
	av_frame_free(av_frame);
	av_free_packet(av_pkg);

	(*env)->ReleaseStringUTFChars(env,filename,c_path);
}
