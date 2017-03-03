#include "../../include/androidnames.h"
#include "../../include/enginelayer.h"
#include "../../include/enginecallback.h"
#include <jni.h>

extern "C"
{
	JNIEXPORT jint JNICALL JNIEVENT_______(JNIEnv *env,jobject obj, jint id, jintArray arg)
	{
		return _engineprivate::EngineLayerEvent(env,obj,id,arg);
	}

	JNIEXPORT jstring JNICALL JNISTRINGEVENT_(JNIEnv *env,jobject obj, jint id, jobjectArray arg)
	{
		return _engineprivate::EngineLayerStringEvent(env,obj,id,arg);
	}
	
	JNIEXPORT void JNICALL JNISETUP_______(JNIEnv* env, jobject obj)
	{
		return _engineprivate::EngineLayerSetupJNI(env,obj);
	}
}
