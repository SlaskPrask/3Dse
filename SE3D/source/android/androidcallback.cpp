#include "../../include/enginelayer.h"
#include "../../include/enginecallback.h"
#include "../../include/androidnames.h"

using namespace _engineprivate;
using namespace _ENGINESPACE;

jint _engineprivate::EngineLayerEvent(JNIEnv *env,jobject obj, jint id, jintArray arg)
{
	if (arg==0)
	return EngineLayer::event(id,0,0);

	jsize size=env->GetArrayLength(arg);
	jint *arr=env->GetIntArrayElements(arg,0);
	int *value=new int[size];
	for(int i=0;i<size;i++)
	value[i]=arr[i];
	env->ReleaseIntArrayElements(arg,arr,0);

	int result=EngineLayer::event(id,size,value);

	delete[] value;
	return result;
}

jstring _engineprivate::EngineLayerStringEvent(JNIEnv *env,jobject obj, jint id, jobjectArray arg)
{
	jstring result;
	if (arg==0)
	{
		result=env->NewStringUTF(EngineLayer::stringEvent(id,0,NULL).c_str());
		return result;
	}

	jsize size=env->GetArrayLength(arg);
	std::string *value=new std::string[size];
	for(int i=0;i<size;i++)
	value[i]=(const char*)env->GetStringUTFChars((jstring)env->GetObjectArrayElement(arg,i),0);

	result=env->NewStringUTF(EngineLayer::stringEvent(id,size,value).c_str());

	delete[] value;
	return result;
}

void _engineprivate::EngineLayerSetupJNI(JNIEnv* env, jobject obj)
{
	EngineLayer::setJVM(env);
}

bool _engineprivate::CallbackUnsetFullscreen()
{
	JNIEnv *e=EngineLayer::getEnv();
	jclass c=*EngineLayer::instance()->getEngineLayer();

	jmethodID m=e->GetStaticMethodID(c,"unsetFullscreen","()I");
	jint set=e->CallStaticIntMethod(c,m);

	return (set!=0);
}

bool _engineprivate::CallbackSetFullscreen()
{
	JNIEnv *e=EngineLayer::getEnv();
	jclass c=*EngineLayer::instance()->getEngineLayer();

	jmethodID m=e->GetStaticMethodID(c,"setFullscreen","()I");
	jint set=e->CallStaticIntMethod(c,m);

	return (set!=0);
}

void _engineprivate::CallbackSetTitle(const std::string &s)
{
	JNIEnv *e=EngineLayer::getEnv();
	jstring str=e->NewStringUTF(s.c_str());
	jclass c=*EngineLayer::instance()->getEngineLayer();

	jmethodID m=e->GetStaticMethodID(c,"setTitle","(Ljava/lang/String;)V");
	e->CallStaticVoidMethod(c,m,str);
	e->DeleteLocalRef(str);
}

void _engineprivate::CallbackPrintLog(const std::string &s)
{
	JNIEnv *e=EngineLayer::getEnv();
	jstring str=e->NewStringUTF(s.c_str());
	jclass c=*EngineLayer::instance()->getEngineLayer();

	jmethodID m=e->GetStaticMethodID(c,"log","(Ljava/lang/String;)V");
	e->CallStaticVoidMethod(c,m,str);
	e->DeleteLocalRef(str);
}

void _engineprivate::CallbackSetOrientation(int orientation)
{
	JNIEnv *e=EngineLayer::getEnv();
	jclass c=*EngineLayer::instance()->getEngineLayer();

	jmethodID m=e->GetStaticMethodID(c,"setOrientation","(I)V");
	e->CallStaticVoidMethod(c,m,orientation);
}

GLuint _engineprivate::CallbackLoadPNG(const std::string &s,int *width,int *height,bool threaded,GLuint *destination)
{
	JNIEnv *e=EngineLayer::getEnv();
	jstring str=e->NewStringUTF(s.c_str());
	jclass c=*EngineLayer::instance()->getAssetManager();
	
	jmethodID m=e->GetStaticMethodID(c,"open","(Ljava/lang/String;)Landroid/graphics/Bitmap;");
	jobject png=e->CallStaticObjectMethod(c,m,str);

	if (png==NULL)
	return 0;
	
	m=e->GetStaticMethodID(c,"getWidth","(Landroid/graphics/Bitmap;)I");
	*width=e->CallStaticIntMethod(c,m,png);
	m=e->GetStaticMethodID(c,"getHeight","(Landroid/graphics/Bitmap;)I");
	*height=e->CallStaticIntMethod(c,m,png);
	
	jintArray pixdata=e->NewIntArray((*width)*(*height));
	m=e->GetStaticMethodID(c,"getData","(Landroid/graphics/Bitmap;[I)V");
	e->CallStaticVoidMethod(c,m,png,pixdata);
	
	jint *pixels=e->GetIntArrayElements(pixdata,0);
	GLubyte *data=new GLubyte[(*width)*(*height)*4];

	//max white (skip ARGB->ABGR)
	for (int i=0;i<(*width)*(*height);i++)
	{
		data[i*4+0]=(0xFF&((pixels[i]&0x00FF0000)>>16));
		data[i*4+1]=(0xFF&((pixels[i]&0x0000FF00)>>8));
		data[i*4+2]=(0xFF&((pixels[i]&0x000000FF)));
		data[i*4+3]=(0xFF&((pixels[i]&0xFF000000)>>24));
	}
	
	e->ReleaseIntArrayElements(pixdata,pixels,0);
	m=e->GetStaticMethodID(c,"close","(Landroid/graphics/Bitmap;)V");
	e->CallStaticVoidMethod(c,m,png);

	e->DeleteLocalRef(str);
	e->DeleteLocalRef(pixdata);

	if (!threaded)
	{
		GLuint tex=0;

		_engine::generateTexture(&tex,*width,*height,data,GL_RGBA);

		delete[] data;
		return tex;
	}
	else
	{
		EngineLayer::pushLoaderData(destination,*width,*height,data,GL_RGBA);
		return 1;//temp value
	}
	return 0;
}

GLuint _engineprivate::CallbackLoadFont(const std::string &s,int size,Font *fnt,int startc,int camount,bool threaded,GLuint *destination)
{
	JNIEnv *e=EngineLayer::getEnv();
	jstring str=e->NewStringUTF(s.c_str());
	jclass c=*EngineLayer::instance()->getAssetManager();
	
	jfloatArray measures=e->NewFloatArray(3);
	jfloatArray widths=e->NewFloatArray(_FONT_CHARACTERS);
	
	jmethodID m=e->GetStaticMethodID(c,"loadFontSetup","(Ljava/lang/String;I[F[F)I");
	jint texsize=e->CallStaticIntMethod(c,m,str,size,measures,widths);
	if (texsize<=0)
	return 0;

	jfloat *meas=e->GetFloatArrayElements(measures,0);
	jfloat *charwidth=e->GetFloatArrayElements(widths,0);
	jintArray pixdata=e->NewIntArray(texsize*texsize);

	m=e->GetStaticMethodID(c,"loadFont","(Ljava/lang/String;IIIIF[I)V");
	e->CallStaticVoidMethod(c,m,str,size,texsize,startc,camount,meas[0],pixdata);

	jint *pixels=e->GetIntArrayElements(pixdata,0);
	GLubyte *data=new GLubyte[texsize*texsize*4];
	
	//max white (skip ARGB->ABGR)
	for(int i=0;i<texsize*texsize;i++)
	{
		data[i*4+0]=0xFF;
		data[i*4+1]=0xFF;
		data[i*4+2]=0xFF;
		data[i*4+3]=(0xFF&((pixels[i]&0xFF000000)>>24));
	}
	
	EngineLayer::instance()->setFontData(fnt,startc,camount,meas,charwidth,texsize);
	e->ReleaseFloatArrayElements(widths,charwidth,0);
	e->ReleaseIntArrayElements(pixdata,pixels,0);
	e->ReleaseFloatArrayElements(measures,meas,0);

	e->DeleteLocalRef(str);
	e->DeleteLocalRef(measures);
	e->DeleteLocalRef(widths);
	e->DeleteLocalRef(pixdata);

	if (!threaded)
	{
		GLuint tex=0;

		_engine::generateTexture(&tex,texsize,texsize,data,GL_RGBA);

		delete[] data;
		return tex;
	}
	else
	{
		EngineLayer::pushLoaderData(destination,texsize,texsize,data,GL_RGBA);
		return 1;//temp value
	}
	return 0;
}

void _engineprivate::CallbackMessage(const std::string &s)
{
	JNIEnv *e=EngineLayer::getEnv();
	jstring str=e->NewStringUTF(s.c_str());
	
	jclass c=*EngineLayer::instance()->getEngineLayer();
	jmethodID m=e->GetStaticMethodID(c,"message","(Ljava/lang/String;)V");
	e->CallStaticVoidMethod(c,m,str);
	e->DeleteLocalRef(str);
}

void _engineprivate::CallbackRequestInput(int type)
{
	JNIEnv *e=EngineLayer::getEnv();
	jclass c=*EngineLayer::instance()->getEngineLayer();
	
	jmethodID m=e->GetStaticMethodID(c,"requestInput","(I)V");
	e->CallStaticVoidMethod(c,m,type);
}

void _engineprivate::CallbackSetFPS(int fps)
{
	JNIEnv *e=EngineLayer::getEnv();
	jclass c=*EngineLayer::instance()->getEngineLayer();
	
	jmethodID m=e->GetStaticMethodID(c,"setFPS","(I)V");
	e->CallStaticVoidMethod(c,m,fps);
}

int _engineprivate::CallbackLoadSound(const std::string &s)
{
	JNIEnv *e=EngineLayer::getEnv();
	jstring str=e->NewStringUTF(s.c_str());
	jclass c=*EngineLayer::instance()->getEngineLayer();
	
	jmethodID m=e->GetStaticMethodID(c,"sndLoad","(Ljava/lang/String;)I");
	int snd=e->CallStaticIntMethod(c,m,str);
	e->DeleteLocalRef(str);
	return snd;
}

void _engineprivate::CallbackUnloadSound(int i)
{
	JNIEnv *e=EngineLayer::getEnv();
	jclass c=*EngineLayer::instance()->getEngineLayer();
	
	jmethodID m=e->GetStaticMethodID(c,"sndUnload","(I)V");
	e->CallStaticVoidMethod(c,m,i);
}

int _engineprivate::CallbackPlaySound(int i,float l,float r,int prio,int loops,float speed)
{
	JNIEnv *e=EngineLayer::getEnv();
	jclass c=*EngineLayer::instance()->getEngineLayer();
	
	jmethodID m=e->GetStaticMethodID(c,"sndPlay","(IFFIIF)I");
	return e->CallStaticIntMethod(c,m,i,l,r,prio,loops,speed);
}

void _engineprivate::CallbackPauseSound(int i)
{
	JNIEnv *e=EngineLayer::getEnv();
	jclass c=*EngineLayer::instance()->getEngineLayer();
	
	jmethodID m=e->GetStaticMethodID(c,"sndPause","(I)V");
	e->CallStaticVoidMethod(c,m,i);
}

void _engineprivate::CallbackUnpauseSound(int i)
{
	JNIEnv *e=EngineLayer::getEnv();
	jclass c=*EngineLayer::instance()->getEngineLayer();
	
	jmethodID m=e->GetStaticMethodID(c,"sndUnpause","(I)V");
	e->CallStaticVoidMethod(c,m,i);
}

void _engineprivate::CallbackStopSound(int i)
{
	JNIEnv *e=EngineLayer::getEnv();
	jclass c=*EngineLayer::instance()->getEngineLayer();
	
	jmethodID m=e->GetStaticMethodID(c,"sndStop","(I)V");
	e->CallStaticVoidMethod(c,m,i);
}

void _engineprivate::CallbackSpeedSound(int i,float spd)
{
	JNIEnv *e=EngineLayer::getEnv();
	jclass c=*EngineLayer::instance()->getEngineLayer();
	
	jmethodID m=e->GetStaticMethodID(c,"sndSpeed","(IF)V");
	e->CallStaticVoidMethod(c,m,i,spd);
}

void _engineprivate::CallbackVolumeSound(int i,float left,float right)
{
	JNIEnv *e=EngineLayer::getEnv();
	jclass c=*EngineLayer::instance()->getEngineLayer();
	
	jmethodID m=e->GetStaticMethodID(c,"sndVolume","(IFF)V");
	e->CallStaticVoidMethod(c,m,i,left,right);
}

void _engineprivate::CallbackCloseApplication()
{
	JNIEnv *e=EngineLayer::getEnv();
	jclass c=*EngineLayer::instance()->getEngineLayer();
	
	jmethodID m=e->GetStaticMethodID(c,"closeApplication","()V");
	e->CallStaticVoidMethod(c,m);
}
