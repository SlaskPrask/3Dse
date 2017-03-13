#pragma once

#include "enginesettings.h"
#include "object.h"
#include "enginewrapper.h"
#include <vector>

#define SCENE \
private:\
friend class _engineprivate::EngineLayer;\
inline static std::string *_sceneName()\
{\
static std::string _name=EngineLayer::getClassName(_OBJECT_NAME);\
return &_name;\
}\
inline static unsigned int *_sceneId()\
{\
	static unsigned int _id=_engine::obtainScnId();\
	return &_id;\
}\
static _engine::SceneListing _listing(_sceneName());\
template<class T>\
inline static void _restart(T *p)\
{\
	new T();\
}\
public:\
virtual inline unsigned int sceneId()\
{\
	return *_sceneId();\
}\
virtual inline void restart()\
{\
	_restart(this);\
}\
virtual inline std::string sceneName()\
{\
	return *_sceneName();\
}\
private:

namespace _engineprivate
{
	class EngineLayer;
}

namespace _ENGINESPACE
{
	using namespace _engineprivate;
	
	class Scene
	{
		friend class _engineprivate::EngineLayer;
		
		private:
		std::string _objname;
		unsigned int _objid;
		std::vector<Object*> _objects;
		void _deleteObjects();
		bool _deathMark;
		bool _deleted;
		virtual bool _getDeleted()
		{
			return _deleted;
		}
		
		public:
		int width,height;
		virtual inline std::string sceneName()
		{
			return "";
		}
		virtual inline unsigned int sceneId()
		{
			return -1;
		}
		virtual inline void restart()
		{
			return;
		}
		inline void setSize(int w,int h)
		{
			width=w;
			height=h;
		}
		Scene();
		virtual void run();
		virtual ~Scene();
	};
}
