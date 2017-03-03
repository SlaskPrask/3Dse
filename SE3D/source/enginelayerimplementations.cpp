#include "../include/enginelayer.h"
#include "../include/shader.h"
#include "../include/platformgraphics.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

using namespace _engineprivate;
using namespace _ENGINESPACE;

void(*EngineLayer::gameEndFunc)()=NULL;
void(*EngineLayer::gameStartFunc)()=NULL;
void(*EngineLayer::gameWindowResizeFunc)()=NULL;
void(*EngineLayer::gameFocusGainFunc)()=NULL;
void(*EngineLayer::gameFocusLossFunc)()=NULL;

#ifdef ANDROID
const bool EngineLayer::mobile=1;
#else
const bool EngineLayer::mobile=0;
#endif
const int EngineLayer::DEFAULT_DEPTH=0;
const int EngineLayer::DEFAULT_DEPTH_RANGE=10000;
const unsigned int EngineLayer::DEFAULT_COLLISION_OBJECTS=10;
const int EngineLayer::DEFAULT_COLLISION_SIZE=1024;
const int EngineLayer::DEFAULT_COLLISION_DEPTH=5;
#ifndef ANDROID
const int EngineLayer::MAX_KEYBOARD_KEYS=sf::Keyboard::Key::KeyCount;
#else
const int EngineLayer::MAX_KEYBOARD_KEYS=1;
#endif

EngineLayer::EngineLayer()
{
	Log::log("Engine","Started");
	{
		std::string verstr="Version: ";
		verstr+=ENGINEVERSION;
		#ifdef DEBUG
		#ifdef STANDALONE
		verstr+="S";
		#endif
		#ifdef MOBILE
		verstr+="M";
		#endif
		verstr+="D";
		#endif
		verstr+=" ";
		#ifdef WINDOWS
		verstr+="Windows ";
		#endif
		#ifdef ANDROID
		verstr+="Android ";
		#endif
		#ifdef LINUX
		verstr+="Linux ";
		#endif
		#ifdef MAC
		verstr+="Mac ";
		#endif
		#ifdef UNIX
		verstr+="(Unix) ";
		#endif
		#ifdef DEBUG
		verstr+=_DEFSTR(_ENGINESPACE);
		verstr+="::";
		verstr+=_DEFSTR(_ENGINECLASS);
		#endif
		Log::log("Engine",verstr);
	}

	if (!Log::isPrinting())
	Log::notify("Engine","Unable to write log file");

	pullResources=0;

	//graphics
	fps=60;
	defaultCamera=new Camera();
	activeCamera=defaultCamera;
	queueCamera=0;
	reqWidth=reqHeight=width=height=1;
	#ifndef ANDROID
	defaultResolutionWidth=sf::VideoMode::getDesktopMode().width;
	defaultResolutionHeight=sf::VideoMode::getDesktopMode().height;
	#else
	defaultResolutionWidth=0;
	defaultResolutionHeight=0;
	#endif
	r=g=b=0;
	squareData[0]=0;
	squareData[1]=0;
	squareData[2]=1;
	squareData[3]=0;
	squareData[4]=0;
	squareData[5]=1;
	squareData[6]=1;
	squareData[7]=1;
	texData[0]=0;
	texData[1]=0;
	texData[2]=1;
	texData[3]=0;
	texData[4]=0;
	texData[5]=1;
	texData[6]=1;
	texData[7]=1;
	regionW=regionH=1;//?
	blackBars=1;
	horBars=verBars=0;
	widthRatio=heightRatio=1;
	dominantRatio=0;
	resDirectory="";
	workingDirectory="./";
	
	GLfloat vnear=1,vfar=-1,vtop=0,vleft=0,vright=width,vbottom=height;
	ortho[0]=2.0f/(vright-vleft);
	ortho[1]=0;
	ortho[2]=0;
	ortho[3]=-(vright+vleft)/(vright-vleft);
	ortho[4]=0;
	ortho[5]=2.0f/(vtop-vbottom);
	ortho[6]=0;
	ortho[7]=-(vtop+vbottom)/(vtop-vbottom);
	ortho[8]=0;
	ortho[9]=0;
	ortho[10]=-2.0f/(vfar-vnear);
	ortho[11]=-(vfar+vnear)/(vfar-vnear);
	ortho[12]=0;
	ortho[13]=0;
	ortho[14]=0;
	ortho[15]=1;
	rot[0]=cos(0);
	rot[1]=-sin(0);
	rot[2]=0;
	rot[3]=0;
	rot[4]=sin(0);
	rot[5]=cos(0);
	rot[6]=0;
	rot[7]=0;
	rot[8]=0;
	rot[9]=0;
	rot[10]=1;
	rot[11]=0;
	rot[12]=0;
	rot[13]=0;
	rot[14]=0;
	rot[15]=1;
	textured=0;
	
	posTrans[0]=posTrans[1]=0;
	colorSet[0]=colorSet[1]=colorSet[2]=colorSet[3]=1;
	colorizationSet[0]=colorizationSet[1]=colorizationSet[2]=colorizationSet[3]=0;
	halfsize[0]=halfsize[1]=0;

	#ifndef ANDROID
	settings.depthBits=24;
	settings.stencilBits=8;
	settings.antialiasingLevel=4;
	settings.majorVersion=3;
	settings.minorVersion=0;//3
	resolutions=sf::VideoMode::getFullscreenModes();

	#ifdef DEBUG
	if (resolutions.empty())
	Log::notify("Engine","0 resolutions available");
	else
	Log::log("Engine",std::string(to_string(resolutions.size()))+" resolutions available");
	#endif
	window=NULL;
	fontLib=new FT_Library();
	if (FT_Init_FreeType(fontLib))
	{
		Log::error("Engine","Unable to initialize fonts");
		fontLib=0;
	}
	#ifdef DEBUG
	else
	Log::log("Engine","Fonts initialized");
	#endif
	#endif
	vsync=1;
	title=_DEFAULT_TITLE;
	cursorOn=1;
	
	//engine
	masterCollision=new Collision(0,0,DEFAULT_COLLISION_SIZE,DEFAULT_COLLISION_SIZE,0,NULL);
	forcedRelease=0;
	firstDepth=0;
	scene=0;
	switchingScenes=0;
	frameTime=1;
	maxFrameTime=1;
	handleObjDeleted=0;
	started=0;
	running=1;
	stopped=0;
	backButton=0;
	windowResized=0;
	focusGained=focusLost=0;
	resumed=paused=0;
	focus=1;
	keyboardinput="";
	srand((unsigned int)time(NULL));

	key=new int[MAX_KEYBOARD_KEYS];
	keyboardstr="";
	touchClick=0;
	touchTranslation=0;
	touchNoPosition=0;
	touchNoPositionX=-1;
	touchNoPositionY=-1;
	anykeycount=0;
	anykeyreleased=anykeypressed=0;
	anykeyheld=0;

	for (int i=0;i<MAX_KEYBOARD_KEYS;i++)
	key[i]=0;

	for(int i=0;i<_MAX_MOUSES;i++)
	{
		mouseX[i]=mouseY[i]=0;
		mouseTX[i]=mouseTY[i]=0;
		mousePresses[i]=-1;
		mouseWheelUp[i]=0;
		mouseWheelDown[i]=0;
		for(int j=0;j<_MAX_MOUSE_BUTTONS;j++)
		mouseState[i][j]=MouseEvent::Unheld;
	}

	//debug
	#ifdef DEBUG
	debugShowCollision=0;
	debugShowTouchables=0;
	#endif

	Log::log("Engine","Initialized");
}

int EngineLayer::runGame(int argc,char* argv[])
{
	#ifndef ANDROID
	instance()->state.windowResized=1;
	if (argc>0)
	{
		instance()->workingDirectory=getDirectory(argv[0]);
		if (workingDirectory=="")
		workingDirectory="./";
		DEBUGFUNC(Log::log("Engine",std::string("Working directory: ")+workingDirectory));
		for (int i=1;i<argc-1;i++)
		instance()->launchParameter(argv[i]);
	}
	DEBUGFUNC(Log::log("Engine","Game starting"));
	run(1);
	#endif
	return 0;
}

void EngineLayer::launchParameter(char* str)
{
	DEBUGFUNC(Log::log("Engine",std::string("Launch parameter: ")+str));
	//TODO: implement
}

std::string EngineLayer::getDirectory(const std::string &file)
{
	unsigned int pos=file.find_last_of("/\\");
	if (pos!=std::string::npos)
	return file.substr(0,pos)+"/";
	else
	return "";
}

std::string EngineLayer::resourceDirectory(const std::string &file)
{
	return std::string(resDirectory+file);
}

void EngineLayer::setResourceDirectory(const std::string &str)
{
	//disabled for android
	#ifndef ANDROID
	resDirectory=workingDirectory;
	resDirectory+=str;
	resDirectory+='/';
	DEBUGFUNC(Log::log("Engine",std::string("Resource directory changed to: ")+resDirectory));
	#else
	DEBUGFUNC(Log::notify("Engine","Resource directory not changed"));
	#endif
}

void EngineLayer::addKeyboardChar(std::string *s,bool newlines)
{
	for (unsigned int i=0;i<keyboardstr.size();i++)
	{
		if (keyboardstr[i]<32)
			switch (keyboardstr[i])
			{
				default:
					break;
				case '\n'://newline
				case '\r'://carriage return
					if (newlines)
					*s+='\n';
					break;
				case '\b'://backspace
					if (!s->empty())
					*s=s->substr(0,s->size() - 1);
					break;
			}
		else
			switch (keyboardstr[i])
			{
				case 127://delete
					break;
				default:
					*s+=keyboardstr[i];
					break;
			}
	}
}

void EngineLayer::setRotation(GLfloat angle,GLfloat w,GLfloat h)
{
	halfsize[0]=w/2.0f;
	halfsize[1]=h/2.0f;
	angle=angle*M_PI/180.0f;
	rot[0]=cos(angle);
	rot[1]=-sin(angle);
	rot[4]=sin(angle);
	rot[5]=cos(angle);
	glUniformMatrix4fv(drawRotation,1,0,rot);
	glUniform2fv(drawHalfSize,1,halfsize);
}

void EngineLayer::setOrtho(GLfloat left,GLfloat top,GLfloat right,GLfloat bottom)
{
	ortho[0]=2.0f/(right-left);
	ortho[3]=-(right+left)/(right-left);
	ortho[5]=2.0f/(top-bottom);
	ortho[7]=-(top+bottom)/(top-bottom);
}

void EngineLayer::closeGraphics()
{
	#ifdef ANDROID
	return;
	#else
	if (!window)
	return;
	DEBUGFUNC(Log::log("Graphics","Closing graphics"));
	window->close();
	printGLErrors("Closing");
	#endif
}

bool EngineLayer::setFullscreen(int w,int h)
{
	reqWidth=w;
	reqHeight=h;
	#ifdef ANDROID
	if (w>h)
	setOrientation(0);
	else
	if (h>w)
	setOrientation(1);
	else
	setOrientation(2);
	return CallbackSetFullscreen();
	#else
	bool valid=0;
	for (unsigned int i=0;i<resolutions.size();i++)
	if (resolutions[i].width==w&&resolutions[i].height==h)
	{
		valid=1;
		break;
	}

	if (!valid)
	{
		Log::notify("Engine",std::string("Unable to go fullscreen, incompatible resolution ")+to_string(w)+","+to_string(h)+"g iven");
		return 0;
	}

	closeGraphics();

	#ifdef MAC
	window = new sf::RenderWindow(sf::VideoMode(w,h),getTitle(),sf::Style::Fullscreen);
	#else
	window = new sf::RenderWindow(sf::VideoMode(w,h),getTitle(),sf::Style::Fullscreen,settings);
	#endif
	printGLErrors("Window creation");

	if (!window)
	{
		Log::error("Graphics","Unable to go fullscreen, window failed");
		return 0;
	}
	else
	{
		Log::log("Graphics",std::string("Changed to fullscreen ")+to_string(w)+","+to_string(h));
		setSize(w,h);
		return 1;
	}
	#endif
}

bool EngineLayer::setWindowed(int w,int h)
{
	reqWidth=w;
	reqHeight=h;
	#ifdef ANDROID
	if (w>h)
	setOrientation(0);
	else
	if (h>w)
	setOrientation(1);
	else
	setOrientation(2);
	return CallbackUnsetFullscreen();
	#else
	closeGraphics();

	#ifdef MAC
	window = new sf::RenderWindow(sf::VideoMode(w,h),getTitle(),sf::Style::Default);
	#else
	window = new sf::RenderWindow(sf::VideoMode(w,h),getTitle(),sf::Style::Default,settings);
	#endif
	printGLErrors("Window creation");

	if (!window)
	{
		Log::error("Graphics","Unable to go windowed, window failed");
		return 0;
	}
	else
	{
		Log::log("Graphics",std::string("Changed to windowed ")+to_string(w)+","+to_string(h));
		setSize(w,h);
		return 1;
	}
	#endif
}

bool EngineLayer::setFullscreenWindowed()
{
	reqWidth=defaultResolutionWidth;
	reqHeight=defaultResolutionHeight;
	#ifdef ANDROID
	return CallbackSetFullscreen();
	#else
	

	closeGraphics();

	#ifdef MAC
	window = new sf::RenderWindow(sf::VideoMode(w,h),getTitle(),sf::Style::None);
	#else
	window = new sf::RenderWindow(sf::VideoMode(defaultResolutionWidth,defaultResolutionHeight),getTitle(),sf::Style::None,settings);
	#endif
	printGLErrors("Window creation");

	window->setPosition(sf::Vector2i(0,0));

	if (!window)
	{
		Log::error("Graphics","Unable to go fullscreen windowed, window failed");
		return 0;
	}
	else
	{
		Log::log("Graphics",std::string("Changed to fullscreen windowed ")+to_string(defaultResolutionWidth)+","+to_string(defaultResolutionHeight));
		setSize(defaultResolutionWidth,defaultResolutionHeight);
		return 1;
	}
	#endif
}

void EngineLayer::pushLoaderData(GLuint *destination,int texwidth,int texheight,GLubyte *data,GLenum type)
{
	EngineLayer *i=instance();

	i->loadlock.lock();
	i->loaderdata.push_back(LoaderData(destination,texwidth,texheight,data,type));
	i->pullResources=1;
	i->loadlock.unlock();
}

void EngineLayer::loaderLoad()
{
	if (!loader.load())
	{
		Log::error("Engine","Loader thread failed.");
		loader.forceLoad();
	}
}

void EngineLayer::reapplyGL()
{
	#ifndef ANDROID
	if (window)
	{
		setVSync(vsync);
		setFPS(fps);
		setMouseCursor(cursorOn);

		window->setActive();
	}
	else
	Log::error("Graphics",std::string("Setting graphics without a window "));
	#endif

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	DEBUGFUNC(printGLErrors("GL Alpha"));
	glEnable(GL_BLEND);
	DEBUGFUNC(printGLErrors("GL Blend"));
	printGLErrors("GL settings");
}

void EngineLayer::initGL()
{
	#ifndef ANDROID
	glewExperimental=TRUE;
	GLenum result=glewInit();
	if (result!=GLEW_OK)
	{
		Log::error("Graphics",std::string("Unable initialize GLEW ")+(char*)glewGetString(GLEW_VERSION)+": "+(const char*)glewGetErrorString(result));
	}

	#ifdef DEBUG
	else
		Log::log("Graphics",std::string("Initialized GLEW ")+(const char*)glewGetString(GLEW_VERSION));
	#endif
	printGLErrors("Glew initialization");
	#endif

	drawProgram=glProgram(VERTEX_SHADER,FRAGMENT_SHADER);
	printGLErrors("GL program");
	drawTrans=glGetUniformLocation(drawProgram,"ftrans");
	printGLErrors("GL var ftrans");
	drawOffset=glGetUniformLocation(drawProgram,"foffset");
	printGLErrors("GL var foffset");
	drawProjection=glGetUniformLocation(drawProgram,"fprojection");
	printGLErrors("GL var fprojection");
	drawRotation=glGetUniformLocation(drawProgram,"frotation");
	printGLErrors("GL var frotation");
	drawColor=glGetUniformLocation(drawProgram,"fcolor");
	printGLErrors("GL var fcolor");
	drawTex=glGetUniformLocation(drawProgram,"ftex");
	printGLErrors("GL var ftex");
	drawColorization=glGetUniformLocation(drawProgram,"fcolorization");
	printGLErrors("GL var fcolorization");
	drawTextured=glGetUniformLocation(drawProgram,"ftextured");
	printGLErrors("GL var ftextured");
	drawHalfSize=glGetUniformLocation(drawProgram,"fhalfsize");
	printGLErrors("GL var fhalfsize");
	glViewport(0,0,width,height);
	printGLErrors("GL viewport");
}

GLuint EngineLayer::glProgram(const char* vShader,const char* fShader)
{
	GLuint vertexShader=glShader(vShader,GL_VERTEX_SHADER);
	if (!vertexShader)
	Log::error("Graphics","Unable create vertex shader");
	printGLErrors("Vertex shader");
	
	GLuint fragmentShader=glShader(fShader,GL_FRAGMENT_SHADER);
	if (!vertexShader)
	Log::error("Graphics","Unable create fragment shader");
	
	GLuint program=glCreateProgram();
	GLint success=0;

	glAttachShader(program,vertexShader);
	glAttachShader(program,fragmentShader);
	glLinkProgram(program);

	glGetProgramiv(program,GL_LINK_STATUS,&success);

	if (success==GL_FALSE)
	{
		GLint len=0;
		glGetProgramiv(program,GL_INFO_LOG_LENGTH,&len);
		GLchar *log=new GLchar[len+1];
		glGetProgramInfoLog(program,len,NULL,log);

		Log::error("Graphics",std::string("Program link error: ")+log);
		delete[] log;
		return 0;
	}

	return program;
}

GLuint EngineLayer::glShader(const char* str,GLenum type)
{
	GLuint shader=glCreateShader(type);
	GLint success=0;
	
	int l[1]={(int)(std::string(str).length())};
	glShaderSource(shader,1,&str,l);
	printGLErrors("GL shader source");
	glCompileShader(shader);
	printGLErrors("GL shader compile");

	glGetShaderiv(shader,GL_COMPILE_STATUS,&success);

	if (success==GL_FALSE)
	{
		GLint len=0;
		glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&len);
		GLchar *log=new GLchar[len+1];
		glGetShaderInfoLog(shader,len,NULL,log);

		Log::error("Graphics",std::string("Shader compiler error: ")+log);
		delete[] log;
		return 0;
	}

	return shader;
}

Camera* EngineLayer::getCamera()
{
	return (activeCamera==defaultCamera?NULL:activeCamera);
}

void EngineLayer::setCamera(Camera *cam)
{
	if (cam!=activeCamera)
	{
		refreshCamera();
		updateRatios();
		if (cam==NULL)
		activeCamera=defaultCamera;
		else
		activeCamera=cam;
	}
}

void EngineLayer::setSize(int w,int h)
{
	width=w;
	height=h;
	defaultCamera->setSize(w,h);
	
	updateRatios();
	
	refreshCamera();

	reapplyGL();
}

void EngineLayer::updateRatios()
{
	widthRatio=(double)width/activeCamera->getWidth();
	heightRatio=(double)height/activeCamera->getHeight();
	
	dominantRatio=(activeCamera->getWidth()/activeCamera->getHeight()>(double)width/(double)height);
}

void EngineLayer::setRenderSize()
{
	queueCamera=0;

	glViewport(0,0,width,height);
	DEBUGFUNC(printGLErrors("GL viewport size"));
	
	if (blackBars)
	{
		double camRatio=activeCamera->getWidth()/activeCamera->getHeight();
		double winRatio=(double)width/(double)height;
		if (camRatio>winRatio)
		{
			regionW=activeCamera->getWidth();
			regionH=regionW/winRatio;
			verBars=(regionH-activeCamera->getHeight())/2.0f;
			horBars=0;
		}
		else
		{
			regionH=activeCamera->getHeight();
			regionW=regionH*winRatio;
			verBars=0;
			horBars=(regionW-activeCamera->getWidth())/2.0f;
		}
		setOrtho(0,0,regionW,regionH);
		DEBUGFUNC(printGLErrors("GL ortho"));
	}
	else
	{
		setOrtho(0,0,activeCamera->getWidth(),activeCamera->getHeight());
		DEBUGFUNC(printGLErrors("GL ortho"));
		regionW=activeCamera->getWidth();
		regionH=activeCamera->getHeight();
	}
}

double EngineLayer::getCameraX()
{
	return (activeCamera?activeCamera->x:defaultCamera->x);
}
double EngineLayer::getCameraY()
{
	return (activeCamera?activeCamera->y:defaultCamera->y);
}
double EngineLayer::getCameraW()
{
	return (activeCamera?activeCamera->getWidth():defaultCamera->getWidth());
}
double EngineLayer::getCameraH()
{
	return (activeCamera?activeCamera->getHeight():defaultCamera->getHeight());
}

void EngineLayer::drawBegin()
{

	if (queueCamera)
	setRenderSize();

	posOffset[0]=(GLfloat)(activeCamera->x-horBars);
	posOffset[1]=(GLfloat)(activeCamera->y-verBars);
	halfsize[0]=halfsize[1]=0;

	#ifndef ANDROID
	//if (window)
	//window->clear();
	#endif
	glClearColor(r,g,b,1.0f);
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	DEBUGFUNC(printGLErrors("Draw clear"));
	glUseProgram(drawProgram);
	DEBUGFUNC(printGLErrors("Draw program"));
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,squareData);
	DEBUGFUNC(printGLErrors("Draw square"));
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,texData);
	DEBUGFUNC(printGLErrors("Draw data"));
	glUniform2fv(drawTrans,1,posTrans);
	DEBUGFUNC(printGLErrors("Draw position"));
	glUniform2fv(drawOffset,1,posOffset);
	DEBUGFUNC(printGLErrors("Draw offset"));
	glUniform2fv(drawHalfSize,1,halfsize);
	DEBUGFUNC(printGLErrors("Draw halfsize"));
	glUniform1i(drawTex,0);
	DEBUGFUNC(printGLErrors("Draw tex"));
	glUniformMatrix4fv(drawProjection,1,0,ortho);
	DEBUGFUNC(printGLErrors("Draw ortho"));
	glUniformMatrix4fv(drawRotation,1,0,rot);
	DEBUGFUNC(printGLErrors("Draw rotation"));
	glEnableVertexAttribArray(0);
	DEBUGFUNC(printGLErrors("Draw attrib 1"));
	glEnableVertexAttribArray(1);
	DEBUGFUNC(printGLErrors("Draw attrib 2"));
	printGLErrors("Draw begin");
}

void EngineLayer::drawEnd()
{
	if (blackBars)
	{
		posOffset[0]=posOffset[1]=0;
		glUniform2fv(drawOffset,1,posOffset);
		if (horBars>verBars)
		{
			drawRectangle(0,0,horBars,regionH,0,0,0,0,1);
			drawRectangle(regionW-horBars,0,horBars,regionH,0,0,0,0,1);
		}
		else
		{
			drawRectangle(0,0,regionW,verBars,0,0,0,0,1);
			drawRectangle(0,regionH-verBars,regionW,verBars,0,0,0,0,1);
		}
	}

	#ifndef ANDROID
	if (window)
	window->display();
	printGLErrors("Draw end");
	#endif
}

void EngineLayer::printGLErrors(std::string pos)
{
	#ifdef DEBUG
	pos+=" error: ";
	GLenum result=glGetError();
	while (result!=GL_NO_ERROR)
	{
		#ifdef ANDROID
		//todo ERRORSTR
		Log::error("Graphics",pos+to_string(result));
		#else
		Log::error("Graphics",pos+(const char*)gluErrorString(result));
		#endif
		result=glGetError();
	}
	#else
	GLenum result=glGetError();
	if (result!=GL_NO_ERROR)
	Log::error("Graphics",pos+(const char*)gluErrorString(result));
	#endif
}

void EngineLayer::reloadSprites()
{
	DEBUGFUNC(Log::log("Engine","Reloading sprites"));
	for(std::vector<Sprite*>::iterator it=loadedSprites.begin();it!=loadedSprites.end();++it)
	(*it)->reload();
}

void EngineLayer::listLoadedSprite(Sprite *s)
{
	std::vector<Sprite*>::iterator it=std::find(loadedSprites.begin(),loadedSprites.end(),s);
	if (it==loadedSprites.end())//optional if
	loadedSprites.push_back(s);
}

void EngineLayer::unlistLoadedSprite(Sprite *s)
{
	std::vector<Sprite*>::iterator it=std::find(loadedSprites.begin(),loadedSprites.end(),s);
	if (it!=loadedSprites.end())
	loadedSprites.erase(it);
}

void EngineLayer::reloadFonts()
{
	DEBUGFUNC(Log::log("Engine","Reloading fonts"));
	for(std::vector<Font*>::iterator it=loadedFonts.begin();it!=loadedFonts.end();++it)
	(*it)->reload();
}

void EngineLayer::listLoadedFont(Font *s)
{
	std::vector<Font*>::iterator it=std::find(loadedFonts.begin(),loadedFonts.end(),s);
	if (it==loadedFonts.end())//optional if
	loadedFonts.push_back(s);
}

void EngineLayer::unlistLoadedFont(Font *s)
{
	std::vector<Font*>::iterator it=std::find(loadedFonts.begin(),loadedFonts.end(),s);
	if (it!=loadedFonts.end())
	loadedFonts.erase(it);
}

double EngineLayer::getScreenRatio()
{
	//if (blackbars)
	return dominantRatio?widthRatio:heightRatio;
}

void EngineLayer::setColorization(double r,double g,double b)
{
	colorizationSet[0]=r;
	colorizationSet[1]=g;
	colorizationSet[2]=b;
	//colorizationSet[3]=0;
	glUniform4fv(drawColorization,1,colorizationSet);
}

void EngineLayer::drawRectangle(double x,double y,double w,double h,double rot,double r,double g,double b,double a)
{
	setRotation(rot,w,h);
	
	setColor(r,g,b,a);
	
	textured=0;
	posTrans[0]=x;
	posTrans[1]=y;
	squareData[0]=squareData[4]=0;
	squareData[1]=squareData[3]=0;
	squareData[2]=squareData[6]=w;
	squareData[5]=squareData[7]=h;
	glUniform2fv(drawTrans,1,posTrans);
	glUniform1i(drawTextured,textured);
	glDrawArrays(GL_TRIANGLE_STRIP,0,4);
	
	restoreColor();
}

void EngineLayer::drawPoly(double x1,double y1,double x2,double y2,double x3,double y3,double rot,double r,double g,double b,double a)
{
	setRotation(rot);
	double cx=(x1+x2+x3)/3.0f;
	double cy=(y1+y2+y3)/3.0f;
	
	setColor(r,g,b,a);
	
	textured=0;
	posTrans[0]=cx;
	posTrans[1]=cy;
	squareData[0]=x1-cx;
	squareData[1]=y1-cy;
	squareData[2]=x2-cx;
	squareData[3]=y2-cy;
	squareData[4]=x3-cx;
	squareData[5]=y3-cy;
	glUniform2fv(drawTrans,1,posTrans);
	glUniform1i(drawTextured,textured);
	glDrawArrays(GL_TRIANGLE_STRIP,0,3);
	
	restoreColor();
}

void EngineLayer::drawSpriteFinal(Sprite* sprite,double x,double y,double w,double h,double texx1,double texy1,double texx2,double texy2,double texx3,double texy3,double texx4,double texy4,double rot,double r,double g,double b,double a)
{
	setRotation(rot,w,h);
	
	setColor(r,g,b,a);
	
	textured=1;
	posTrans[0]=x;
	posTrans[1]=y;
	squareData[0]=squareData[4]=0;
	squareData[1]=squareData[3]=0;
	squareData[2]=squareData[6]=w;
	squareData[5]=squareData[7]=h;
	texData[0]=texx1;
	texData[1]=texy1;
	texData[2]=texx2;
	texData[3]=texy2;
	texData[4]=texx3;
	texData[5]=texy3;
	texData[6]=texx4;
	texData[7]=texy4;
	glActiveTexture(GL_TEXTURE0);
	sprite->bind();
	glUniform1i(drawTex,0);
	glUniform2fv(drawTrans,1,posTrans);
	glUniform1i(drawTextured,textured);
	glDrawArrays(GL_TRIANGLE_STRIP,0,4);
	
	restoreColor();
}

void EngineLayer::drawText(Font *font,const std::string &str,double x,double y,double size,double lineSpacing,double r,double g,double b,double a,int align)
{
	setColor(r,g,b,a);
	
	GLfloat lineHeight=(GLfloat)font->lineh;
	GLfloat scale=font->ratio;
	GLfloat advscale=size/(double)font->size;
	GLfloat xoff=font->xoff;
	GLfloat yoff=font->yoff;
	
	textured=1;
	squareData[0]=squareData[4]=xoff*advscale;
	squareData[1]=squareData[3]=yoff*advscale;
	squareData[2]=squareData[6]=xoff*advscale+size*scale;
	squareData[5]=squareData[7]=yoff*advscale+size*scale;
	
	std::vector<GLfloat> linew;
	int c=0,lines=0;
	std::stringstream ss(str);
	std::string ln;
	while (std::getline(ss,ln,'\n'))
	{
		GLfloat offset=0;
		for (unsigned int i=0;i<ln.size();i++)
		{
			c=ln.at(i);
			if (c<0||c>=_FONT_CHARACTERS)
			continue;
			
			offset+=font->charw[c];
		}
		linew.push_back(offset*advscale);
		lines++;
	}
	
	if (lines==0)
	align=0;
	
	int line=0;
	switch (align)
	{
		default:
		case _FONT_ALIGN_LEFT:
			posTrans[0]=(GLfloat)x;
			break;
		case _FONT_ALIGN_MIDDLE:
			posTrans[0]=(GLfloat)x-linew[0]/2.0f;
			break;
		case _FONT_ALIGN_RIGHT:
			posTrans[0]=(GLfloat)x-linew[0];
			break;
	}
	posTrans[1]=(GLfloat)(y-size*scale+font->fonth*advscale);
	for(unsigned int i=0;i<str.length();i++)
	{
		c=str.at(i);
		
		if (c<0||c>=_FONT_CHARACTERS)
		continue;
		
		if (c=='\n')
		{
			line++;
			switch (align)
			{
				default:
				case _FONT_ALIGN_LEFT:
					posTrans[0]=x;
					break;
				case _FONT_ALIGN_MIDDLE:
					posTrans[0]=x-linew[line]/2.0f;
					break;
				case _FONT_ALIGN_RIGHT:
					posTrans[0]=x-linew[line];
					break;
			}
			posTrans[1]=(GLfloat)(y-size*scale+font->fonth*advscale+line*(font->lineh+lineSpacing)*advscale);
			continue;
		}
		texData[0]=texData[4]=(GLfloat)((c%16)*0.0625);
		texData[1]=texData[3]=(GLfloat)(floorf((c%_FONT_SET_CHARACTERS)/16)*0.0625);
		texData[2]=texData[6]=(GLfloat)((c%16+1)*0.0625);
		texData[5]=texData[7]=(GLfloat)(floorf((c%_FONT_SET_CHARACTERS)/16+1)*0.0625);
		glActiveTexture(GL_TEXTURE0);
		if (c%_FONT_SET_CHARACTERS)
		font->bind1();
		else
		font->bind2();
		glUniform1i(drawTex,0);
		glUniform2fv(drawTrans,1,posTrans);
		glUniform1i(drawTextured,textured);
		glDrawArrays(GL_TRIANGLE_STRIP,0,4);
		posTrans[0]+=font->charw[c]*advscale;
	}
	
	restoreColor();
}

void EngineLayer::setFontData(Font *font,int startc,int camount,float *meas,float *charwidth,int texsize)
{
	font->setData(startc,camount,meas,charwidth,texsize);
}

void EngineLayer::setGameStartFunc(void (*func)())
{
	EngineLayer::gameStartFunc=func;
}

void EngineLayer::setGameEndFunc(void(*func)())
{
	EngineLayer::gameEndFunc=func;
}

void EngineLayer::setGameWindowResizeFunc(void(*func)())
{
	EngineLayer::gameWindowResizeFunc=func;
}

void EngineLayer::setGameFocusGainFunc(void(*func)())
{
	EngineLayer::gameFocusGainFunc=func;
}

void EngineLayer::setGameFocusLossFunc(void(*func)())
{
	EngineLayer::gameFocusLossFunc=func;
}

void EngineLayer::createObject(Object *o)
{
	objects.add(o);
	
	for(std::vector<Tag*>::iterator it=activeTags.begin();it!=activeTags.end();++it)
	o->addTag(*it);
	
	if (scene)
	{
		o->_persistent=0;
		o->_scene=scene;
		tieObjectToScene(scene,o);
	}
}

ResourceSet* EngineLayer::createResourceSet(ResourceSet *set)
{
	set->engine_id=resourcesets.size();
	resourcesets.push_back(set);
	return set;
}

ResourceSet* EngineLayer::resourceSet(unsigned int i)
{
	if (i<0||i>=resourcesets.size())
	return NULL;
	
	return resourcesets[i];
}

void EngineLayer::resolveUnloadQueue()
{
	while(unload_sets.size())
	{
		unload_sets.back()->_unload();
		unload_sets.pop_back();
	}
}

void EngineLayer::resolveDepthQueue()
{
	while(depthQueue.size())
	{
		attachDepth(depthQueue.back()->_copyDepth());
		depthQueue.pop_back();
	}
}

void EngineLayer::queueDepth(Object *o)
{
	std::vector<Object*>::iterator it=std::find(depthQueue.begin(),depthQueue.end(),o);
	if (it==depthQueue.end())
	depthQueue.push_back(o);
}

void EngineLayer::resolveDepthChangeQueue()
{
	while(depthChangeQueue.size())
	{
		depthChangeQueue.back()->_performDepthMove();
		depthChangeQueue.pop_back();
	}
}

void EngineLayer::queueDepthChange(Object *o)
{
	std::vector<Object*>::iterator it=std::find(depthChangeQueue.begin(),depthChangeQueue.end(),o);
	if (it==depthChangeQueue.end())
	depthChangeQueue.push_back(o);
}

void EngineLayer::detachDepth(Object *o)
{
	for(std::vector<Object*>::iterator it=depthQueue.begin();it!=depthQueue.end();++it)
	if (o==(*it))
	{
		depthQueue.erase(it);
		return;
	}

	if (o->_depthItem==firstDepth)
	firstDepth=o->_depthItem->getNext();
	
	delete (o->_depthItem);
}

void EngineLayer::attachDepth(Object *o)
{
	if (!firstDepth)
	firstDepth=new DepthItem(o);
	else
	(new DepthItem(o))->addAbove(firstDepth);
}

void EngineLayer::switchScene(Scene *scn)
{
	switchingScenes=1;
	if (scene)
	{
		scene->_deathMark=1;
		scene->_deleted=1;
		delete scene;
	}
	scene=scn;
}

void EngineLayer::beginTag(Tag *t) 
{
	std::vector<Tag*>::iterator it=std::find(activeTags.begin(),activeTags.end(),t);
	if (it==activeTags.end())
	activeTags.push_back(t);
}
void EngineLayer::endTag(Tag *t)
{
	std::vector<Tag*>::iterator it=std::find(activeTags.begin(),activeTags.end(),t);
	if (it!=activeTags.end())
	activeTags.erase(it);
}

void EngineLayer::removeTouchable(Touchable *t)
{
	std::vector<Touchable*>::iterator it=std::find(touchables.begin(),touchables.end(),t);
	if (it!=touchables.end())
	touchables.erase(it);
}

void EngineLayer::parseTouchables()
{
	if (touchables.empty())
	return;
	
	for(std::vector<Touchable*>::iterator it=touchables.begin();it!=touchables.end();++it)
	{
		if ((*it)->mouse!=-1)
		{
			if (getMouseIdle((*it)->mouse))
			(*it)->hook(-1);
			else
			(*it)->inside=(getMouseTranslatedX((*it)->mouse)>=(*it)->x&&getMouseTranslatedX((*it)->mouse)<(*it)->x+(*it)->w&&getMouseTranslatedY((*it)->mouse)>=(*it)->y&&getMouseTranslatedY((*it)->mouse)<(*it)->y+(*it)->h);
		}
		(*it)->fixPosition();
	}
	
	int t;
	Touchable *touch=NULL;
	for(int i=0;i<_MAX_MOUSES;i++)
	{
		t=getMousePresses(i);
		if (t==-1)
		break;
		
		for(std::vector<Touchable*>::iterator it=touchables.begin();it!=touchables.end();++it)
		if ((*it)->enabled)
		if (((*it)->mouse==-1)&&(touch==NULL||touch->getDepth()>(*it)->getDepth()))
		if (getMouseTranslatedX(t)>=(*it)->x&&getMouseTranslatedX(t)<(*it)->x+(*it)->w&&getMouseTranslatedY(t)>=(*it)->y&&getMouseTranslatedY(t)<(*it)->y+(*it)->h)
		touch=(*it);
		
		if (touch!=NULL)
		touch->hook(t);
	}
}

void EngineLayer::deleteAllObjects()
{
	DEBUGFUNC(Log::log("Engine","Deleting all objects"));
	LinkedList<Object> *obj=objects.first();
	while(obj)
	{
		((Object*)obj)->_deathMark=1;
		delete obj;
		obj=objects.first();
	}
}

void EngineLayer::deleteAllSets()
{
	DEBUGFUNC(Log::log("Engine","Deleting all resource sets"));
	for (unsigned int i=0;i<resourcesets.size();i++)
	if (resourcesets[i]!=NULL)
	{
		if (resourcesets[i]->loads!=0)
		Log::log("Engine","Resource set "+to_string(resourcesets[i]->engine_id)+"("+to_string(resourcesets[i])+") left loaded");
		resourcesets[i]->_deathMark=1;
		delete resourcesets[i];
	}
	resourcesets.clear();
}

void EngineLayer::deleteExceptionResourceSet(ResourceSet *set)
{
	for (unsigned int i=0;i<resourcesets.size();i++)
	if (resourcesets[i]==set)
	{
		resourcesets[i]=NULL;
		break;
	}
}

void EngineLayer::objAddTag(Object* o,Tag* t)
{
	if (t->attachObj(o))
	{
		o->_tags.push_back(t);
		o->_refreshTagRuns(t->isRunning());
		o->_refreshTagDraws(t->isDrawing());
	}
}

void EngineLayer::objRemoveTag(Object* o,Tag* t)
{
	if (t->detachObj(o))
	{
		std::vector<Tag*>::iterator it=std::find(o->_tags.begin(),o->_tags.end(),t);
		o->_tags.erase(it);
		o->_refreshTagRuns(1);
		o->_refreshTagDraws(1);
	}
}

void EngineLayer::objUnlinkTag(Object* o,Tag* t)
{
	std::vector<Tag*>::iterator it=std::find(o->_tags.begin(),o->_tags.end(),t);
	o->_tags.erase(it);
	o->_refreshTagRuns(1);
	o->_refreshTagDraws(1);
}

void EngineLayer::untieObjectFromScene(Scene *scn,Object *obj)
{
	std::vector<Object*>::iterator it=std::find(scn->_objects.begin(),scn->_objects.end(),obj);
	scn->_objects.erase(it);
}

void EngineLayer::setMouseCursor(bool enabled)
{
	#ifndef ANDROID
	cursorOn=enabled;
	if (window)
	window->setMouseCursorVisible(enabled);
	#endif
}

void EngineLayer::setVSync(bool enabled)
{
	#ifndef ANDROID
	vsync=enabled;
	if (window)
	window->setVerticalSyncEnabled(enabled);
	#else
	//unsupported
	vsync=1;
	#endif
}

void EngineLayer::setTitle(const std::string &label)
{
	#ifndef ANDROID
	title=label;
	if (window)
	window->setTitle(title);
	#else
	CallbackSetTitle(title);
	#endif
}

double EngineLayer::strToD(const std::string &str)
{
	double i;
	std::stringstream ss(str);
	ss>>i;
	return i;
}

int EngineLayer::strToI(const std::string &str)
{
	int i;
	std::stringstream ss(str);
	ss>>i;
	return i;
}

EngineLayer::~EngineLayer()
{
	Log::log("Engine","Closing");

	#ifndef ANDROID
	if (window)
	{
		closeGraphics();
		delete window;
		window=0;
	}
	if (fontLib)
	{
		FT_Done_FreeType(*fontLib);
		delete fontLib;
		fontLib=0;
	}
	#endif
	delete defaultCamera;

	delete[] key;
	Log::log("Engine","Finished");
}


#ifdef DEBUG
void EngineLayer::dumpObjects()
{
	Log::log("DUMP","OBJECTS:");
	int i=0;
	LinkedList<Object> *obj = objects.first();
	while (obj)
	{
		LinkedList<Object> *nextObj= obj->getNext();
		std::ostringstream s;
		s << to_string(i) << ": " << ((Object*)obj)->objectName() << " (" << (Object*)obj << ")";
		Log::log("DUMP",s.str().c_str());
		i++;
		obj = nextObj;
	}

	Log::log("DUMP","");
}
void EngineLayer::dumpObjectDepths()
{
	Log::log("DUMP","OBJECT DEPTHS:");
	DepthItem *di,*diNext;
	int i=0;
	if (di=firstDepth)
		while (di)
		{
			diNext = di->getNext();
			std::ostringstream s;
			s << to_string(i) << ": " << di << " " << di->get()->objectName();
			s << " (" << di->get() << ":" << di->getDepth() << ") ";
			Log::log("DUMP",s.str().c_str());
			i++;
			di=diNext;
		}

	Log::log("DUMP","");
}
void EngineLayer::dumpObjects(unsigned int id)
{
	Log::log("DUMP","OBJECTS:");
	int i=0;
	LinkedList<Object> *obj=objects.first();
	while (obj)
	if (((Object*)obj)->objectId()==id)
	{
		LinkedList<Object> *nextObj=obj->getNext();
		std::ostringstream s;
		s << to_string(i) << ": " << ((Object*)obj)->objectName() << " (" << (Object*)obj << ")";
		Log::log("DUMP",s.str().c_str());
		i++;
		obj=nextObj;
	}

	Log::log("DUMP","");
}
void EngineLayer::dumpDepths()
{
	Log::log("DUMP","DEPTHS:");
	DepthItem *di,*diNext;
	int i=0;
	if (di=firstDepth)
	while (di)
	{
		diNext=di->getNext();
		std::ostringstream s;
		//i
		s << to_string(i) << ": ";

		//prev
		if (di->getPrevious())
		s << di->getPrevious() << "(" << di->getPrevious()->get() << ":" << di->getPrevious()->getDepth() << ") -> ";
		else
		s << ((Object*)NULL) << "(" << ((Object*)NULL) << ":-) -> ";

		//this
		s << di << "(" << di->get() << ":" << di->getDepth() << ")";

		//next
		if (di->getNext())
		s << " -> " << di->getNext() << "(" << di->getNext()->get() << ":" << di->getNext()->getDepth() << ")";
		else
		s << " -> " << ((Object*)NULL) << "(" << ((Object*)NULL) << ":-)";

		Log::log("DUMP",s.str().c_str());
		i++;

		di=diNext;
	}

	Log::log("DUMP","");
}
void EngineLayer::dumpDepthQueue()
{
	Log::log("DUMP","DEPTH QUEUE:");
	for (unsigned int i=0;i<depthQueue.size();i++)
	{
		std::ostringstream s;
		s << to_string(i) << ": " << depthQueue[i] << " " << depthQueue[i]->_depth << "->" << depthQueue[i]->_qdepth;
		Log::log("DUMP",s.str().c_str());
		depthQueue[i];
	}

	Log::log("DUMP","");
}
void EngineLayer::dumpDepthChangeQueue()
{
	Log::log("DUMP","DEPTH CHANGE QUEUE:");
	for (unsigned int i=0;i<depthChangeQueue.size();i++)
	{
		std::ostringstream s;
		s << to_string(i) << ": " << depthChangeQueue[i] << " " << depthChangeQueue[i]->_depth << "->" << depthChangeQueue[i]->_qdepth;
		Log::log("DUMP",s.str().c_str());
		depthChangeQueue[i];
	}

	Log::log("DUMP","");
}
void EngineLayer::dumpObj(Object *obj)
{
	std::ostringstream s;
	s << "OBJECT: " << obj->objectName() << " (" << obj << ")";
	Log::log("DUMP",s.str().c_str());
	s.str("");
	s << "DEPTH OBJ: " << obj->_depthItem;
	Log::log("DUMP",s.str().c_str());
	s.str("");
	s << "DEPTH: " << to_string(obj->_depth) << "->" << to_string(obj->_qdepth);
	Log::log("DUMP",s.str().c_str());
	s.str("");
	s << "X,Y: " << to_string(obj->x) << "," << to_string(obj->y);
	Log::log("DUMP",s.str().c_str());
	s.str("");
	s << (obj->_visible?"VISIBLE":"INVISIBLE");
	Log::log("DUMP",s.str().c_str());

	Log::log("DUMP","");
}
void EngineLayer::debugToggleCollision()
{
	debugShowCollision=!debugShowCollision;
}
void EngineLayer::debugToggleTouchables()
{
	debugShowTouchables=!debugShowTouchables;
}
void EngineLayer::debugHandler()
{
	#ifndef ANDROID
	if (getKeyHeld(sf::Keyboard::Key::LControl)||getKeyHeld(sf::Keyboard::Key::RControl))
	{
		if (getKeyPress(sf::Keyboard::Key::F12))
		dumpObjects();
		if (getKeyPress(sf::Keyboard::Key::F11))
		dumpObjectDepths();
		if (getKeyPress(sf::Keyboard::Key::F10))
		dumpDepths();

		if (getKeyPress(sf::Keyboard::Key::F8))
		debugToggleCollision();
		if (getKeyPress(sf::Keyboard::Key::F7))
		debugToggleTouchables();

		if (getKeyPress(sf::Keyboard::Key::F1))
		if (scene)
		scene->restart();
	}
	else
	if (getKeyHeld(sf::Keyboard::Key::LShift)||getKeyHeld(sf::Keyboard::Key::RShift))
	{
		if (getKeyPress(sf::Keyboard::Key::F12))
		dumpDepthChangeQueue();
		if (getKeyPress(sf::Keyboard::Key::F11))
		dumpDepthQueue();
	}
	#endif
}
#endif