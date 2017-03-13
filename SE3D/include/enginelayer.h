#pragma once

#include <list>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <mutex>
#include "platform.h"
#ifdef ANDROID
#include <jni.h>
#else
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#endif
#include "platformgraphics.h"
#include "enginesettings.h"
#include "log.h"
#include "sprite.h"
#include "sound.h"
#include "font.h"
#include "listhandle.h"
#include "tag.h"
#include "camera.h"
#include "depthitem.h"
#include "object.h"
#include "scene.h"
#include "resource.h"
#include "resourceset.h"
#include "touchable.h"
#include "enginecallback.h"
#include "collision.h"
#include "hitbox.h"
#include "collisionobject.h"
#include "loader.h"

#ifdef ANDROID
#define _MAX_MOUSES (10)
#define _MAX_MOUSE_BUTTONS (1)
#define _MAX_JOYSTICKS (1)
#else
#define _MAX_MOUSES (1)
#define _MAX_MOUSE_BUTTONS (7)
#define _MAX_JOYSTICKS (8)
#endif
#define _DEFAULT_TITLE "Game"

//release num,dev stage,patch,[0=alpha 1=beta 2=candidate 3=release]
#define ENGINEVERSION "1.1.0.0"

namespace _engineprivate
{
	using namespace _ENGINESPACE;
	
	struct MouseEvent
	{
		enum MouseState{Unheld,Press,Held,Release,Move};
		int which;
		int type;
		int x,y;
		int button;
		MouseEvent()
		{}
		MouseEvent(int fw,int ft,int fx,int fy,int b=0):which(fw),type(ft),x(fx),y(fy),button(b)
		{}
	};

	struct LayerState
	{
		bool backButton;
		bool windowResized;
		bool closed;
		bool paused;
		bool resumed;
		bool focusGained;
		bool focusLost;
		std::string keyboardinput;
		std::vector<MouseEvent> mouseEvents;
		LayerState()
		{
			backButton=windowResized=closed=paused=resumed=focusGained=focusLost=0;
			keyboardinput="";
		}
	};

	struct LoaderData
	{
		GLuint *destination;
		GLubyte *data;
		int texwidth,texheight;
		GLenum type;
		LoaderData(GLuint *dst,int w,int h,GLubyte *d,GLenum t):destination(dst),data(d),texwidth(w),texheight(h),type(t)
		{};
	};

	class EngineLayer
	{
		private:
		#ifdef ANDROID
		JavaVM *jvm;
		#endif
		EngineLayer();
		~EngineLayer();
		
		//engine
		LayerState state;
		bool started,running,stopped;
		bool focus;

		bool backButton;
		bool windowResized;
		bool closed;
		bool paused,resumed;
		bool focusGained,focusLost;
		std::string keyboardinput;
		bool forcedRelease;
		
		int mouseX[_MAX_MOUSES],mouseY[_MAX_MOUSES],mouseState[_MAX_MOUSES][_MAX_MOUSE_BUTTONS];
		double mouseTX[_MAX_MOUSES],mouseTY[_MAX_MOUSES];
		int mousePresses[_MAX_MOUSES];
		
		volatile bool pullResources;
		Loader loader;
		std::mutex loadlock;
		std::vector<LoaderData> loaderdata;
		ListHandle<Object> objects;
		std::vector<Object*> depthQueue;
		std::vector<Object*> depthChangeQueue;
		std::vector<Tag*> activeTags;
		DepthItem *firstDepth;
		Collision *masterCollision;
		Scene *scene;
		bool switchingScenes;
		double frameTime,maxFrameTime,frameMultiplier;
		#ifdef DEBUG
		double debugTimeScale;
		int debugTimeStep;
		#endif
		Object *handleObj;
		bool handleObjDeleted;
		std::string resDirectory,workDirectory;
		static void(*gameStartFunc)();
		static void(*gameEndFunc)();
		static void(*gameWindowResizeFunc)();
		static void(*gameFocusGainFunc)();
		static void(*gameFocusLossFunc)();
		std::vector<ResourceSet*> resourcesets;
		std::vector<ResourceSet*> unload_sets;
		std::vector<Touchable*> touchables;
		double strToD(const std::string &str);
		int strToI(const std::string &str);

		void coreStart();
		void coreRun(double delta);
		void coreEnd();

		int mouse_butt[_MAX_MOUSES];
		int mouseWheelUp[_MAX_MOUSES],mouseWheelDown[_MAX_MOUSES];
		int anykeycount;
		double deadzone[_MAX_JOYSTICKS];
		double buttondeadzone[_MAX_JOYSTICKS];

		std::string keyboardstr;

		int *key;
		bool anykeyreleased,anykeypressed;
		int anykeyheld;

		bool touchTranslation;
		int touchClick;
		bool touchNoPosition;
		int touchNoPositionX,touchNoPositionY;
		
		//graphics
		static const int DEFAULT_DEPTH,DEFAULT_DEPTH_RANGE;
		#ifndef ANDROID
		FT_Library *fontLib;
		#endif
		int fps;
		GLfloat ortho[16],rot[16];
		GLfloat posOffset[2],posTrans[2],halfsize[2];
		GLfloat colorSet[4],colorizationSet[4];
		GLfloat squareData[8],texData[8];
		GLuint drawProgram;
		GLuint drawColorization,drawOffset,drawTrans,drawProjection,drawRotation,drawColor,drawHalfSize,drawTex,drawTextured;
		GLint textured;
		GLfloat r,g,b;
		int width,height,reqWidth,reqHeight;
		int defaultResolutionWidth,defaultResolutionHeight;
		Camera *defaultCamera;
		Camera *activeCamera;
		bool queueCamera;
		bool blackBars;
		double horBars,verBars;
		double regionW,regionH;
		double widthRatio,heightRatio;
		bool dominantRatio;
		std::vector<Sprite*> loadedSprites;
		std::vector<Font*> loadedFonts;
		void updateRatios();
		#ifndef ANDROID
		std::vector<sf::VideoMode> resolutions;
		sf::RenderWindow* window;
		sf::ContextSettings settings;
		#endif
		bool cursorOn;
		bool vsync;
		std::string title;
		

		public:
		inline static EngineLayer* instance()
		{
			static EngineLayer i;
			return &i;
		}
		static const int MAX_KEYBOARD_KEYS;

		#ifdef ANDROID
		static void setJVM(JNIEnv *env);
		static JNIEnv* getEnv();
		jclass JNIAssetManager,JNIEngineLayer;
		inline jclass* getAssetManager()
		{
			return &JNIAssetManager;
		}
		inline jclass* getEngineLayer()
		{
			return &JNIEngineLayer;
		}
		#endif
		static int event(int id, int size, int *value);
		static std::string stringEvent(int id, int size, std::string *value);
		static void message(const std::string &m);
		void eventParser();
		
		//engine
		static const bool mobile;
		static const unsigned int DEFAULT_COLLISION_OBJECTS;
		static const int DEFAULT_COLLISION_SIZE;
		static const int DEFAULT_COLLISION_DEPTH;
		int runGame(int argc,char *argv[]);
		void launchParameter(char* str);
		std::string getDirectory(const std::string &file);
		static void setGameStartFunc(void(*func)());
		static void setGameEndFunc(void(*func)());
		static void setGameWindowResizeFunc(void(*func)());
		static void setGameFocusGainFunc(void(*func)());
		static void setGameFocusLossFunc(void(*func)());
		static void pushLoaderData(GLuint *destination,int texwidth,int texheight,GLubyte *data,GLenum type);;
		void run(double delta);
		void switchScene(Scene *scn);
		void createObject(Object *o);
		ResourceSet* createResourceSet(ResourceSet *set);
		ResourceSet* resourceSet(unsigned int i);
		inline void unloadResourceSet(ResourceSet *set)
		{
			unload_sets.push_back(set);
		}
		void deleteExceptionResourceSet(ResourceSet *set);
		void resolveUnloadQueue();
		inline void gameEnd()
		{
			stopped=1;
			running=0;
		}
		void objAddTag(Object* o,Tag* );
		void objRemoveTag(Object* o,Tag* t);
		void objUnlinkTag(Object* o,Tag* t);
		inline void objClearTag(Object *o,Tag* t)
		{
			t->detachObj(o);
		}
		inline void refreshObjTagDraws(Object *o,bool value)
		{
			o->_refreshTagDraws(value);
		}
		inline void refreshObjTagRuns(Object *o,bool value)
		{
			o->_refreshTagRuns(value);
		}
		void beginTag(Tag *t);
		void endTag(Tag *t);
		void resolveDepthQueue();
		void resolveDepthChangeQueue();
		void queueDepth(Object *o);
		void queueDepthChange(Object *o);
		void detachDepth(Object *o);
		void attachDepth(Object *o);
		inline void setFirstDepth(DepthItem *d)
		{
			firstDepth=d;
		}
		inline void deleteScene()
		{
			if (scene)
			{
				scene->_deathMark=1;
				scene->_deleted=1;
				delete scene;
				scene=NULL;
			}
		}
		void deleteAllSets();
		void deleteAllObjects();
		inline void checkDeletedObject(Object* o)
		{
			if (o==handleObj)
			handleObjDeleted=1;
		}
		inline bool isObjectDestroyed(Object* o)
		{
			return o->_getDestroyed();
		}
		inline Scene* getCurrentScene()
		{
			return scene;
		}
		inline void tieObjectToScene(Scene *scn,Object *obj)
		{
			scn->_objects.push_back(obj);
		}
		void untieObjectFromScene(Scene *scn,Object *obj);
		inline void forcePersistent(Object *obj)
		{
			obj->_scene=NULL;
			obj->_persistent=1;
		}
		inline Hitbox* createCollisionBox(Object *obj,double px,double py,double w,double h)
		{
			return new CollisionBox(obj,px,py,w,h);
		}
		inline Hitbox* createCollisionCircle(Object *obj,double px,double py,double r)
		{
			return new CollisionCircle(obj,px,py,r);
		}
		inline Hitbox* createCollisionLine(Object *obj,double px,double py,double dx,double dy)
		{
			return new CollisionLine(obj,px,py,dx,dy);
		}
		inline Hitbox* createCollisionPolygon(Object *obj,double px,double py,double px2,double py2,double px3,double py3)
		{
			return new CollisionPolygon(obj,px,py,px2,py2,px3,py3);
		}
		inline void getObjectCollisions(Hitbox *h,std::vector<Object*> *vec)
		{
			h->collide(vec);
		}
		inline double getFrameTime()
		{
			return frameTime;
		}
		#ifndef DEBUG
		inline double getTimeScale()
		{
			return frameMultiplier;
		}
		#else
		inline double getTimeScale()
		{
			return frameMultiplier*debugTimeScale;
		}
		#endif
		inline void setTimeScale(double d)
		{
			frameMultiplier=d;
		}
		inline bool sceneExiting(Scene *scn)
		{
			return (scn&&scn->_getDeleted());
		}
		void setResourceDirectory(const std::string &str);
		inline std::string getResourceDirectory()
		{
			return resDirectory;
		}
		std::string resourceDirectory(const std::string &file);
		std::string workingDirectory(const std::string &file);
		inline void sendMessage(const std::string &str)
		{
			message(str);
		}
		inline void requestInput(bool newlines=0)
		{
			forcedRelease=1;
			CallbackRequestInput(newlines?1:0);
		}
		inline void requestInputNumber(bool newlines=0)
		{
			forcedRelease=1;
			CallbackRequestInput(newlines?3:2);
		}
		inline void requestInputEmail()
		{
			forcedRelease=1;
			CallbackRequestInput(4);
		}
		inline double getMouseTranslatedX(int i)
		{
			return mouseTX[i];
		}
		inline double getMouseTranslatedY(int i)
		{
			return mouseTY[i];
		}
		inline int getMouseX(int i)
		{
			return mouseX[i];
		}
		inline int getMouseY(int i)
		{
			return mouseY[i];
		}
		inline bool getMousePress(int i,int b=0)
		{
			return (mouseState[i][b]==MouseEvent::Press);
		}
		inline bool getMouseRelease(int i,int b=0)
		{
			return (mouseState[i][b]==MouseEvent::Release);
		}
		inline bool getMouseHeld(int i,int b=0)
		{
			return (mouseState[i][b]==MouseEvent::Held||mouseState[i][b]==MouseEvent::Press);
		}
		inline bool getMouseUnheld(int i,int b=0)
		{
			return (mouseState[i][b]==MouseEvent::Unheld||mouseState[i][b]==MouseEvent::Release);
		}
		inline bool getMouseIdle(int i,int b=0)
		{
			return (mouseState[i][b]==MouseEvent::Unheld);
		}
		inline void getMousePresses(int i[_MAX_MOUSES])
		{
			memcpy(i,mousePresses,sizeof(int)*_MAX_MOUSES);
		}
		inline int getMousePresses(int i)
		{
			return (i<0?-1:(i>=_MAX_MOUSES?-1:mousePresses[i]));
		}
		inline bool getMousePresses()
		{
			return (mousePresses[0]!=-1);
		}
		inline bool getKeyPress(int i)
		{
			return (key[i]==1);
		}
		inline bool getKeyRelease(int i)
		{
			return (key[i]==-1);
		}
		inline bool getKeyHeld(int i)
		{
			return (key[i]==1||key[i]==2);
		}
		inline bool getKeyIdle(int i)
		{
			return (key[i]==-1||key[i]==0);
		}
		inline bool getAnyKeyPress()
		{
			return anykeypressed;
		}
		inline bool getAnyKeyRelease()
		{
			return anykeyreleased;
		}
		void addKeyboardChar(std::string *s,bool newlines=1,unsigned int valuelimit=256);
		inline int getMouseWheelUp(int i)
		{
			return mouseWheelUp[i];
		}
		inline int getMouseWheelDown(int i)
		{
			return mouseWheelDown[i];
		}
		inline bool getBackButton()
		{
			#ifdef ANDROID
			return backButton;
			#else
			return getKeyPress(sf::Keyboard::Escape);
			#endif
		}
		
		//audio
		inline int sndPlay(Sound *sound,double l,double r,int prio,bool loop,double speed)
		{
			return sound->play(l,r,prio,loop,speed);
		}
		inline void sndStop(int i)
		{
			CallbackStopSound(i);
		}
		inline void sndStop(Sound *sound)
		{
			sound->stop();
		}
		inline void sndVolume(int i,double l,double r)
		{
			CallbackVolumeSound(i,(float)l,(float)r);
		}
		inline void sndSpeed(int i,double s)
		{
			CallbackSpeedSound(i,(float)s);
		}
		inline void sndUnpause(int i)
		{
			CallbackUnpauseSound(i);
		}
		inline void sndPause(int i)
		{
			CallbackPauseSound(i);
		}
		inline void setMasterCollision(Collision *cm)
		{
			masterCollision=cm;
		}
		inline Collision* getMasterCollision()
		{
			return masterCollision;
		}
		
		//graphics
		void setFontData(Font *font,int startc,int camount,float *meas,float *charwidth,int texsizew,int texsizeh,int xoff,int yoff)
		{
			font->setData(startc,camount,meas,charwidth,texsizew,texsizeh,xoff,yoff);
		}
		void reloadSprites();
		void reloadFonts();
		void listLoadedSprite(Sprite *s);
		void unlistLoadedSprite(Sprite *s);
		void listLoadedFont(Font *s);
		void unlistLoadedFont(Font *s);
		void setRotation(GLfloat angle,GLfloat w,GLfloat h);
		inline void setRotation(GLfloat angle)
		{
			setRotation(angle,0,0);
		}
		void setOrtho(GLfloat left,GLfloat top,GLfloat right,GLfloat bottom);
		static GLuint glShader(const char* str,GLenum type);
		static GLuint glProgram(const char* vertexShader,const char* fragmentShader);
		void initGL();
		void reapplyGL();
		void setSize(int w,int h);
		void setRenderSize();
		inline void contextLost()
		{
			initGL();
		}
		inline double getVerBar()
		{
			return (blackBars?verBars:0);
		}
		inline double getHorBar()
		{
			return (blackBars?horBars:0);
		}
		inline double getRegionW()
		{
			return regionW;
		}
		inline double getRegionH()
		{
			return regionH;
		}
		Camera *getCamera();
		void setCamera(Camera *cam);
		inline void refreshCamera()
		{
			updateRatios();
			queueCamera=1;
		}
		inline void scaleFit()
		{
			if (!blackBars)
			refreshCamera();
			blackBars=1;
		}
		inline void scaleStretch()
		{
			if (blackBars)
			refreshCamera();
			blackBars=0;
		}
		double getCameraX();
		double getCameraY();
		double getCameraW();
		double getCameraH();
		inline bool getFocusEvent()
		{
			return focusGained;
		}
		inline bool getUnfocusEvent()
		{
			return focusLost;
		}
		inline bool getResizeEvent()
		{
			return windowResized;
		}
		void drawBegin();
		void drawEnd();
		#ifndef ANDROID
		inline FT_Library* getFontLib()
		{
			return fontLib;
		}
		#endif
		inline void setFPS(int f)
		{
			fps=f;
			CallbackSetFPS(fps);
		}
		inline int getFPS()
		{
			return fps;
		}
		inline int getCurrentFPS()
		{
			return (int)(1.0f/frameTime);
		}
		inline void setMaxFrameTime(double max)
		{
			maxFrameTime=max;
		}
		inline double getMaxFrameTime()
		{
			return maxFrameTime;
		}
		inline void setBackgroundColor(double fr,double fg,double fb)
		{
			r=(GLfloat)fr;
			g=(GLfloat)fg;
			b=(GLfloat)fb;
		}
		inline void restoreColor()
		{
			colorSet[0]=colorSet[1]=colorSet[2]=colorSet[3]=1;
			glUniform4fv(drawColor,1,colorSet);
		}
		inline void setColor(double r,double g,double b,double a)
		{
			colorSet[0]=(GLfloat)r;
			colorSet[1]=(GLfloat)g;
			colorSet[2]=(GLfloat)b;
			colorSet[3]=(GLfloat)a;
			glUniform4fv(drawColor,1,colorSet);
		}
		void setColorization(double r,double g,double b);
		void setMouseCursor(bool enabled);
		void setTitle(const std::string &label);
		void setVSync(bool enabled);
		inline bool getMouseCursor()
		{
			return cursorOn;
		}
		inline bool getVSync()
		{
			return vsync;
		}
		inline std::string getTitle()
		{
			return title;
		}
		#ifndef ANDROID
		inline sf::RenderWindow* getWindow()
		{
			return window;
		}
		#endif
		inline int getResolutions()
		{
			#ifdef ANDROID
			return 1;
			#else
			return (unsigned int)resolutions.size();
			#endif
		}
		inline int getResolutionWidth(unsigned int i)
		{
			#ifdef ANDROID
			return getWidth();
			#else
			return (i>=resolutions.size()?0:resolutions[i].width);
			#endif
		}
		inline int getResolutionHeight(unsigned int i)
		{
			#ifdef ANDROID
			return getHeight();
			#else
			return (i>=resolutions.size()?0:resolutions[i].height);
			#endif
		}
		void closeGraphics();
		bool setFullscreen()
		{
			return setFullscreen(defaultResolutionWidth,defaultResolutionHeight);
		}
		bool setWindowed()
		{
			#ifdef ANDROID
			return CallbackUnsetFullscreen();
			#endif
			return 0;
		}
		bool setFullscreen(int w,int h);
		bool setWindowed(int w,int h);
		bool setFullscreenWindowed();
		inline void setOrientation(int orientation)
		{
			CallbackSetOrientation(orientation);
		}
		inline int getWidth()
		{
			return width;
		}
		inline int getHeight()
		{
			return height;
		}
		static std::string getClassName(const char* c)
		{
			std::string s(c);
			unsigned int colon=(unsigned int)s.find_last_of(':')-1;
			unsigned int space=(unsigned int)s.find_last_of(' ',colon)+1;
			return s.substr(space,colon-space);
		}
		template<class T>
		inline unsigned int getObjectId()
		{
			return *T::_objectId();
		}
		template<class T>
		inline unsigned int getSceneId()
		{
			return *T::_sceneId();
		}
		template<class T>
		inline int objectCount()
		{
			int objs=0;
			LinkedList<Object> *objI;
			objI=objects.first();
			while(objI)
			{
				if (((Object*)objI)->objectId()==T::objectId())
				objs++;
				objI=objI->getNext();
			}
			return objs;
		}
		inline int objectCountAll()
		{
			int objs=0;
			LinkedList<Object> *objI;
			objI=objects.first();
			while(objI)
			{
				objs++;
				objI=objI->getNext();
			}
			return objs;
		}
		inline void loaderAdd(ResourceSet *set)
		{
			loader.add(set);
		}
		inline unsigned int loaderItems()
		{
			return loader.getItems();
		}
		inline unsigned int loaderLoadedItems()
		{
			return loader.getLoaded();
		}
		inline void loaderClear()
		{
			loader.clear();
		}
		void loaderLoad();
		inline bool loaderDone()
		{
			bool status;
			loadlock.lock();
			status=!pullResources;
			loadlock.unlock();
			return status&&loader.getDone();
		}
		inline void requirePull()
		{
			loadlock.lock();
			pullResources=1;
			loadlock.unlock();
		}
		inline bool fileExists(const std::string &file)
		{
			return CallbackExistsFile(workingDirectory(file));
		}
		inline bool fileDelete(const std::string &file)
		{
			return CallbackDeleteFile(workingDirectory(file));
		}
		inline bool fileWrite(const std::string &file,const std::string &text)
		{
			return CallbackWriteFile(workingDirectory(file),text);
		}
		inline bool fileRead(const std::string &file,std::string *text)
		{
			return CallbackReadFile(workingDirectory(file),text);
		}
		double getScreenRatio();
		inline void addTouchable(Touchable *t)
		{
			touchables.push_back(t);
		}
		static void printGLErrors(std::string pos);
		void removeTouchable(Touchable *t);
		void parseTouchables();
		void drawRectangle(double x,double y,double w,double h,double rot,double r,double g,double b,double a);
		void drawPoly(double x1,double y1,double x2,double y2,double x3,double y3,double rot,double r,double g,double b,double a);
		void drawSpriteFinal(Sprite* sprite,double x,double y,double w,double h,double texx1,double texy1,double texx2,double texy2,double texx3,double texy3,double texx4,double texy4,double rot,double r,double g,double b,double a);
		inline void drawSprite(Sprite* sprite,double x,double y,double w,double h,double fromx,double fromy,double tox,double toy,double rot,double r,double g,double b,double a)
		{
			drawSpriteFinal(sprite,x,y,w,h,fromx,fromy,tox,fromy,fromx,toy,tox,toy,rot,r,g,b,a);
		}
		//void drawSpritePolyTex(Sprite* sprite,double x1,double y1,double x2,double y2,double x3,double y3,double texx1,double texy1,double texx2,double texy2,double texx3,double texy3,double rot,double r,double g,double b,double a);
		void drawText(Font *font,const std::string &str,double x,double y,double size,double lineSpacing,double r,double g,double b,double a,int align=0);

		//debug
		#ifdef DEBUG
		int debugShowCollision,debugShowTouchables;
		void dumpObjects();
		void dumpObjectDepths();
		void dumpObjects(unsigned int id);
		void dumpTouchables();
		void dumpDepths();
		void dumpDepthQueue();
		void dumpDepthChangeQueue();
		void dumpObj(Object *obj);
		void dumpObjectListing();
		void dumpSceneListing();
		void debugToggleCollision();
		void debugToggleTouchables();
		void debugHandler();
		#endif
	};
}
