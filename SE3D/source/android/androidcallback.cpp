#include "../../include/enginelayer.h"
#include "../../include/enginecallback.h"
#include "../../include/androidnames.h"

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

	int result=_engineprivate::EngineLayer::event(id,size,value);

	delete[] value;
	return result;
}

jstring _engineprivate::EngineLayerStringEvent(JNIEnv *env,jobject obj, jint id, jobjectArray arg)
{
	jstring result;
	if (arg==0)
	{
		result=env->NewStringUTF(_engineprivate::EngineLayer::stringEvent(id,0,NULL).c_str());
		return result;
	}

	jsize size=env->GetArrayLength(arg);
	std::string *value=new std::string[size];
	for(int i=0;i<size;i++)
	value[i]=(const char*)env->GetStringChars((jstring)env->GetObjectArrayElement(arg,i),0);

	result=env->NewStringUTF(_engineprivate::EngineLayer::stringEvent(id,size,value).c_str());

	delete[] value;
	return result;
}

void _engineprivate::EngineLayerSetupJNI(JNIEnv* env, jobject obj)
{
	_engineprivate::EngineLayer::setJVM(env);
}

bool _engineprivate::CallbackExistsFile(const std::string &f)
{
	JNIEnv *e=_engineprivate::EngineLayer::getEnv();
	jstring str=e->NewStringUTF(f.c_str());
	jclass c=*_engineprivate::EngineLayer::instance()->getEngineLayer();

	jmethodID m=e->GetStaticMethodID(c,"existsFile","(Ljava/lang/String;)I");
	int res=e->CallStaticIntMethod(c,m,str);
	e->DeleteLocalRef(str);

	return res;
}

bool _engineprivate::CallbackDeleteFile(const std::string &f)
{
	JNIEnv *e=_engineprivate::EngineLayer::getEnv();
	jstring str=e->NewStringUTF(f.c_str());
	jclass c=*_engineprivate::EngineLayer::instance()->getEngineLayer();

	jmethodID m=e->GetStaticMethodID(c,"deleteFile","(Ljava/lang/String;)I");
	int res=e->CallStaticIntMethod(c,m,str);
	e->DeleteLocalRef(str);

	return res;
}

bool _engineprivate::CallbackWriteFile(const std::string &f,const std::string &s)
{
	JNIEnv *e=_engineprivate::EngineLayer::getEnv();
	jstring str=e->NewStringUTF(f.c_str());
	jbyteArray text=e->NewByteArray(s.length());
	const char *cstr=s.c_str();
	e->SetByteArrayRegion(text,0,s.length(),(const jbyte*)cstr);
	jclass c=*_engineprivate::EngineLayer::instance()->getEngineLayer();

	jmethodID m=e->GetStaticMethodID(c,"writeFile","(Ljava/lang/String;[B)I");
	int res=e->CallStaticIntMethod(c,m,str,text);
	e->DeleteLocalRef(str);
	e->DeleteLocalRef(text);

	return res;
}

bool _engineprivate::CallbackReadFile(const std::string &f,std::string *s)
{
	JNIEnv *e=_engineprivate::EngineLayer::getEnv();
	jstring str=e->NewStringUTF(f.c_str());
	jclass c=*_engineprivate::EngineLayer::instance()->getEngineLayer();

	jintArray inputdata=e->NewIntArray(2);
	jmethodID m=e->GetStaticMethodID(c,"readFile","(Ljava/lang/String;[I)[B");
	jbyteArray text=(jbyteArray)e->CallStaticObjectMethod(c,m,str,inputdata);

	jint *result=e->GetIntArrayElements(inputdata,0);
	int res=result[0];
	int length=result[1];
	e->ReleaseIntArrayElements(inputdata,result,0);

	*s="";
	if (res)
	{
		jbyte *chr=e->GetByteArrayElements(text,NULL);
		for(int i=0;i<result[1];i++)
		*s+=(char)chr[i];
		e->ReleaseByteArrayElements(text,chr,0);
	}

	e->DeleteLocalRef(inputdata);
	e->DeleteLocalRef(text);
	e->DeleteLocalRef(str);

	return res;
}

bool _engineprivate::CallbackUnsetFullscreen()
{
	JNIEnv *e=_engineprivate::EngineLayer::getEnv();
	jclass c=*_engineprivate::EngineLayer::instance()->getEngineLayer();

	jmethodID m=e->GetStaticMethodID(c,"unsetFullscreen","()I");
	jint set=e->CallStaticIntMethod(c,m);

	return (set!=0);
}

bool _engineprivate::CallbackSetFullscreen()
{
	JNIEnv *e=_engineprivate::EngineLayer::getEnv();
	jclass c=*_engineprivate::EngineLayer::instance()->getEngineLayer();

	jmethodID m=e->GetStaticMethodID(c,"setFullscreen","()I");
	jint set=e->CallStaticIntMethod(c,m);

	return (set!=0);
}

void _engineprivate::CallbackSetTitle(const std::string &s)
{
	JNIEnv *e=_engineprivate::EngineLayer::getEnv();
	jstring str=e->NewStringUTF(s.c_str());
	jclass c=*_engineprivate::EngineLayer::instance()->getEngineLayer();

	jmethodID m=e->GetStaticMethodID(c,"setTitle","(Ljava/lang/String;)V");
	e->CallStaticVoidMethod(c,m,str);
	e->DeleteLocalRef(str);
}

void _engineprivate::CallbackSetIcon(const std::string &s)
{
	//TODO
}

void _engineprivate::CallbackPrintLog(const std::string &s)
{
	JNIEnv *e=_engineprivate::EngineLayer::getEnv();
	jstring str=e->NewStringUTF(s.c_str());
	jclass c=*_engineprivate::EngineLayer::instance()->getEngineLayer();

	jmethodID m=e->GetStaticMethodID(c,"log","(Ljava/lang/String;)V");
	e->CallStaticVoidMethod(c,m,str);
	e->DeleteLocalRef(str);
}

void _engineprivate::CallbackSetOrientation(int orientation)
{
	JNIEnv *e=_engineprivate::EngineLayer::getEnv();
	jclass c=*_engineprivate::EngineLayer::instance()->getEngineLayer();

	jmethodID m=e->GetStaticMethodID(c,"setOrientation","(I)V");
	e->CallStaticVoidMethod(c,m,orientation);
}

GLuint _engineprivate::CallbackLoadPNG(const std::string &s,int *width,int *height,bool threaded,GLuint *destination)
{
	JNIEnv *e=_engineprivate::EngineLayer::getEnv();
	jstring str=e->NewStringUTF(s.c_str());
	jclass c=*_engineprivate::EngineLayer::instance()->getAssetManager();
	
	jmethodID m=e->GetStaticMethodID(c,"open","(Ljava/lang/String;)Landroid/graphics/Bitmap;");
	jobject png=e->CallStaticObjectMethod(c,m,str);

	if (png==NULL)
	{
		Log::error("Resources",std::string("Unable to load image file \"")+s+"\"");
		return 0;
	}
	
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
		_engineprivate::EngineLayer::pushLoaderData(destination,*width,*height,data,GL_RGBA);
		return 1;//temp value
	}
	return 0;
}

GLuint _engineprivate::CallbackLoadFont(const std::string &s,int size,Font *fnt,int startc,int camount,int totalchars,bool threaded,GLuint *destination)
{
	JNIEnv *e=_engineprivate::EngineLayer::getEnv();
	jstring str=e->NewStringUTF(s.c_str());
	jclass c=*_engineprivate::EngineLayer::instance()->getAssetManager();

	jfloatArray measures=e->NewFloatArray(3);
	jintArray charsizes=e->NewIntArray(2);
	jintArray offs=e->NewIntArray(2);
	jfloatArray widths=e->NewFloatArray(totalchars);
	
	jmethodID m=e->GetStaticMethodID(c,"loadFontSetup","(Ljava/lang/String;I[I[F[F[II)I");
	e->CallStaticIntMethod(c,m,str,size,charsizes,measures,widths,offs,totalchars);

	jint *charsize=e->GetIntArrayElements(charsizes,0);

	if (charsize[0]<=0||charsize[1]<=0)
	{
		Log::error("Resources",std::string("Unable to load font file \"")+s+"\"");
		return 0;
	}

	int texsizew=charsize[0]*16;
	int texsizeh=charsize[1]*16;

	jfloat *meas=e->GetFloatArrayElements(measures,0);
	jfloat *charwidth=e->GetFloatArrayElements(widths,0);
	jint *xyoff=e->GetIntArrayElements(offs,0);
	jintArray pixdata=e->NewIntArray(texsizew*texsizeh);

	m=e->GetStaticMethodID(c,"loadFont","(Ljava/lang/String;IIIIIFII[I)V");
	e->CallStaticVoidMethod(c,m,str,size,texsizew,texsizeh,startc,camount,meas[0],xyoff[0],xyoff[1],pixdata);

	jint *pixels=e->GetIntArrayElements(pixdata,0);
	GLubyte *data=new GLubyte[texsizew*texsizeh*4];
	
	//max white (skip ARGB->ABGR)
	for(int i=0;i<texsizew*texsizeh;i++)
	{
		data[i*4+0]=0xFF;
		data[i*4+1]=0xFF;
		data[i*4+2]=0xFF;
		data[i*4+3]=(0xFF&((pixels[i]&0xFF000000)>>24));

		/*data[i*4+0]=(0xFF&((pixels[i]&0xFF000000)>>24));
		data[i*4+1]=(0xFF&((pixels[i]&0xFF000000)>>24));
		data[i*4+2]=(0xFF&((pixels[i]&0xFF000000)>>24));
		if (0xFF&((pixels[i]&0xFF000000)>>24)!=0xFF)
		data[i*4+3]=0xFF/3;//*/ //show background
	}
	
	EngineLayer::instance()->setFontData(fnt,startc,camount,meas,charwidth,texsizew,texsizeh,xyoff[0],xyoff[1]);
	e->ReleaseFloatArrayElements(widths,charwidth,0);
	e->ReleaseIntArrayElements(pixdata,pixels,0);
	e->ReleaseIntArrayElements(offs,xyoff,0);
	e->ReleaseFloatArrayElements(measures,meas,0);
	e->ReleaseIntArrayElements(charsizes,charsize,0);

	e->DeleteLocalRef(str);
	e->DeleteLocalRef(offs);
	e->DeleteLocalRef(measures);
	e->DeleteLocalRef(charsizes);
	e->DeleteLocalRef(widths);
	e->DeleteLocalRef(pixdata);

	if (!threaded)
	{
		GLuint tex=0;

		_engine::generateTexture(&tex,texsizew,texsizeh,data,GL_RGBA);

		delete[] data;
		return tex;
	}
	else
	{
		_engineprivate::EngineLayer::pushLoaderData(destination,texsizew,texsizeh,data,GL_RGBA);
		return 1;//temp value
	}
	return 0;
}

void _engineprivate::CallbackMessage(const std::string &s)
{
	JNIEnv *e=_engineprivate::EngineLayer::getEnv();
	jstring str=e->NewStringUTF(s.c_str());
	
	jclass c=*_engineprivate::EngineLayer::instance()->getEngineLayer();
	jmethodID m=e->GetStaticMethodID(c,"message","(Ljava/lang/String;)V");
	e->CallStaticVoidMethod(c,m,str);
	e->DeleteLocalRef(str);
}

void _engineprivate::CallbackRequestInput(int type)
{
	JNIEnv *e=_engineprivate::EngineLayer::getEnv();
	jclass c=*_engineprivate::EngineLayer::instance()->getEngineLayer();
	
	jmethodID m=e->GetStaticMethodID(c,"requestInput","(I)V");
	e->CallStaticVoidMethod(c,m,type);
}

void _engineprivate::CallbackSetFPS(int fps)
{
	JNIEnv *e=_engineprivate::EngineLayer::getEnv();
	jclass c=*_engineprivate::EngineLayer::instance()->getEngineLayer();
	
	jmethodID m=e->GetStaticMethodID(c,"setFPS","(I)V");
	e->CallStaticVoidMethod(c,m,fps);
}

bool _engineprivate::CallbackOpenURL(const std::string &uri)
{
	JNIEnv *e=_engineprivate::EngineLayer::getEnv();
	jstring str=e->NewStringUTF(uri.c_str());
	jclass c=*_engineprivate::EngineLayer::instance()->getEngineLayer();

	jmethodID m=e->GetStaticMethodID(c,"openURL","(Ljava/lang/String;)I");
	int i=e->CallStaticIntMethod(c,m,str);
	e->DeleteLocalRef(str);
	return i!=0;
}

int _engineprivate::CallbackLoadSound(const std::string &s)
{
	JNIEnv *e=_engineprivate::EngineLayer::getEnv();
	jstring str=e->NewStringUTF(s.c_str());
	jclass c=*_engineprivate::EngineLayer::instance()->getEngineLayer();
	
	jmethodID m=e->GetStaticMethodID(c,"sndLoad","(Ljava/lang/String;)I");
	int snd=e->CallStaticIntMethod(c,m,str);
	e->DeleteLocalRef(str);
	return snd;
}

void _engineprivate::CallbackUnloadSound(int i)
{
	JNIEnv *e=_engineprivate::EngineLayer::getEnv();
	jclass c=*_engineprivate::EngineLayer::instance()->getEngineLayer();
	
	jmethodID m=e->GetStaticMethodID(c,"sndUnload","(I)V");
	e->CallStaticVoidMethod(c,m,i);
}

int _engineprivate::CallbackPlaySound(int i,float l,float r,int prio,int loops,float speed)
{
	JNIEnv *e=_engineprivate::EngineLayer::getEnv();
	jclass c=*_engineprivate::EngineLayer::instance()->getEngineLayer();
	
	jmethodID m=e->GetStaticMethodID(c,"sndPlay","(IFFIIF)I");
	return e->CallStaticIntMethod(c,m,i,l,r,prio,loops,speed);
}

void _engineprivate::CallbackPauseSound(int i)
{
	JNIEnv *e=_engineprivate::EngineLayer::getEnv();
	jclass c=*_engineprivate::EngineLayer::instance()->getEngineLayer();
	
	jmethodID m=e->GetStaticMethodID(c,"sndPause","(I)V");
	e->CallStaticVoidMethod(c,m,i);
}

void _engineprivate::CallbackUnpauseSound(int i)
{
	JNIEnv *e=_engineprivate::EngineLayer::getEnv();
	jclass c=*_engineprivate::EngineLayer::instance()->getEngineLayer();
	
	jmethodID m=e->GetStaticMethodID(c,"sndUnpause","(I)V");
	e->CallStaticVoidMethod(c,m,i);
}

void _engineprivate::CallbackStopSound(int i)
{
	JNIEnv *e=_engineprivate::EngineLayer::getEnv();
	jclass c=*_engineprivate::EngineLayer::instance()->getEngineLayer();
	
	jmethodID m=e->GetStaticMethodID(c,"sndStop","(I)V");
	e->CallStaticVoidMethod(c,m,i);
}

void _engineprivate::CallbackSpeedSound(int i,float spd)
{
	JNIEnv *e=_engineprivate::EngineLayer::getEnv();
	jclass c=*_engineprivate::EngineLayer::instance()->getEngineLayer();
	
	jmethodID m=e->GetStaticMethodID(c,"sndSpeed","(IF)V");
	e->CallStaticVoidMethod(c,m,i,spd);
}

void _engineprivate::CallbackVolumeSound(int i,float left,float right)
{
	JNIEnv *e=_engineprivate::EngineLayer::getEnv();
	jclass c=*_engineprivate::EngineLayer::instance()->getEngineLayer();
	
	jmethodID m=e->GetStaticMethodID(c,"sndVolume","(IFF)V");
	e->CallStaticVoidMethod(c,m,i,left,right);
}

void _engineprivate::CallbackCloseApplication()
{
	JNIEnv *e=_engineprivate::EngineLayer::getEnv();
	jclass c=*_engineprivate::EngineLayer::instance()->getEngineLayer();
	
	jmethodID m=e->GetStaticMethodID(c,"closeApplication","()V");
	e->CallStaticVoidMethod(c,m);
}
