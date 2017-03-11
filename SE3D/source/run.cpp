#include "../include/enginelayer.h"

using namespace _engineprivate;
using namespace _ENGINESPACE;

void EngineLayer::run(double delta)
{
	#ifdef ANDROID
	static bool init=1;
	if (init)
	{
		if (_engine::MainFunction::mainFunction())
		{
			int (*mainf)(int,char**)=_engine::MainFunction::mainFunction();
			mainf(0,NULL);
		}
		init=0;
		return;
	}
	if (!started)
	coreStart();
	else
	if (running)
	coreRun(delta);
	
	if (stopped)
	coreEnd();
	#else

	sf::Clock frameClock;
	unsigned long int passedTime;

	while (running)
	{
		if (!started)
		{
			passedTime=1;

			coreStart();
		}
		else
		passedTime=(unsigned long int)frameClock.getElapsedTime().asMicroseconds();

		if (fps>0)
		{
			sf::sleep(sf::microseconds((sf::Int64)(1000000.0f/fps-passedTime)));
			passedTime=(unsigned long int)frameClock.getElapsedTime().asMicroseconds();
		}
		frameClock.restart();
	
		coreRun(delta=(passedTime<=0?0.001:(double)passedTime/1000000.0f));
	}

	if (stopped)
	coreEnd();
	#endif

}

void EngineLayer::coreStart()
{
	started=1;
	Log::enable();
	if (state.windowResized)
	{
		#ifndef ANDROID
		sf::Context context;
		#endif

		Log::log("Graphics",std::string("OpenGL version ")+(const char*)glGetString(GL_VERSION));
		Log::log("Graphics",std::string("GLSL version ")+(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
		initGL();

		DEBUGFUNC(Log::log("Engine","Game started"));

		if (gameStartFunc)
			gameStartFunc();
	}
}

void EngineLayer::coreRun(double delta)
{
	if (maxFrameTime>0&&delta>maxFrameTime)
	delta=maxFrameTime;

	frameTime=delta;

	//debug
	#ifdef DEBUG
	debugHandler();
	#endif

	//loader
	loadlock.lock();
	while(!loaderdata.empty())
	{
		_engine::generateTexture((*loaderdata.begin()).destination,(*loaderdata.begin()).texwidth,(*loaderdata.begin()).texheight,(*loaderdata.begin()).data,(*loaderdata.begin()).type);
		delete[] ((*loaderdata.begin()).data);
		loaderdata.erase(loaderdata.begin());
	}
	pullResources=0;
	loadlock.unlock();

	//events
	eventParser();

	//touchables
	parseTouchables();

	//close
	if (closed)
	{
		running=0;
		stopped=1;
	}

	//resize
	if (windowResized||backButton)
	{
		if (gameWindowResizeFunc)
		gameWindowResizeFunc();
	}

	//focus
	if (focusLost&&gameFocusLossFunc)
	gameFocusLossFunc();
	if (focusGained&&gameFocusGainFunc)
	gameFocusGainFunc();

	switchingScenes=0;
	if (scene)
	scene->run();

	//objects
	DepthItem *di,*diNext;
	LinkedList<Object> *objI,*nextObjI;
	objI=objects.first();
	while (objI)
	{
		handleObj=(Object*)objI;
		handleObjDeleted=0;
		nextObjI=objI->getNext();
		if (!switchingScenes&&handleObj->isRunEnabled())
		handleObj->run();

		if (!handleObjDeleted&&handleObj->_getDestroyed())
		{
			handleObj->_deathMark=1;
			delete handleObj;
		}

		objI=nextObjI;
	}

	//cameras
	Camera *cam=getCamera();
	if (cam)
	cam->doFollow();

	//draw
	resolveDepthQueue();
	resolveDepthChangeQueue();

	#ifndef ANDROID
	if (window)
	#endif
	{
		drawBegin();
		if (di=firstDepth)
		while (di)
		{
			diNext=di->getNext();
			handleObj=di->get();
			handleObjDeleted=0;
			if (handleObj->isDrawEnabled()&&handleObj->_visible&&!handleObj->_getDestroyed()&&!handleObjDeleted)
			handleObj->draw();
			di=diNext;
		}
		#ifdef DEBUG
		if (debugShowCollision)
		masterCollision->draw();
		if (debugShowTouchables)
		for (std::vector<Touchable*>::iterator it=touchables.begin();it!=touchables.end();++it)
		(*it)->draw();
		#endif
		drawEnd();
	}

	//unload sets
	resolveUnloadQueue();
}//running

void EngineLayer::coreEnd()
{
	if (gameEndFunc)
	gameEndFunc();

	DEBUGFUNC(Log::log("Engine","Game finished"));

	loader.exit();
	while(!loaderdata.empty())
	{
		delete[] (*loaderdata.begin()).data;
		loaderdata.erase(loaderdata.begin());
	}

	//empty game end unloads to figure out which were left over
	resolveUnloadQueue();

	//clean game engine
	deleteAllObjects();
	deleteScene();
	deleteAllSets();

	if (!closed)
	{
		DEBUGFUNC(Log::log("Engine","Closing application"));
		CallbackCloseApplication();
	}
}
