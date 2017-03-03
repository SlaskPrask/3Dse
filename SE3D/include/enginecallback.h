#pragma once

#include "enginesettings.h"
#include <string>
#include "platform.h"
#ifdef ANDROID
#include <jni.h>
#endif
#include "platformgraphics.h"
#include "font.h"

namespace _engineprivate
{
	using namespace _ENGINESPACE;
	
	#ifdef ANDROID
	jint EngineLayerEvent(JNIEnv *env,jobject obj, jint id, jintArray arg);
	jstring EngineLayerStringEvent(JNIEnv *env,jobject obj, jint id, jobjectArray arg);
	void EngineLayerSetupJNI(JNIEnv* env, jobject obj);
	#endif

	void CallbackPrintLog(const std::string &s);
	void CallbackSetTitle(const std::string &s);
	void CallbackSetOrientation(int orientation);
	bool CallbackUnsetFullscreen();
	bool CallbackSetFullscreen();
	void CallbackMessage(const std::string &s);
	void CallbackRequestInput(int type);
	void CallbackSetFPS(int fps);
	GLuint CallbackLoadPNG(const std::string &s,int *width,int *height,bool threaded=0,GLuint *destination=0);
	GLuint CallbackLoadFont(const std::string &s,int size,Font *fnt,int startc,int camount,bool threaded=0,GLuint *destination=0);
	int CallbackLoadSound(const std::string &s);
	void CallbackUnloadSound(int i);
	int CallbackPlaySound(int i,float l,float r,int prio,int loops,float speed);
	void CallbackPauseSound(int i);
	void CallbackUnpauseSound(int i);
	void CallbackStopSound(int i);
	void CallbackSpeedSound(int i,float spd);
	void CallbackVolumeSound(int i,float left,float right);
	void CallbackCloseApplication();
}
