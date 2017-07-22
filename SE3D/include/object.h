#pragma once

#include "enginesettings.h"

#include "linkedlist.h"
#include "tag.h"
#include "enginewrapper.h"
#include "hitbox.h"
#include <vector>
#include <string>

namespace _ENGINESPACE
{
	class Object;
}

namespace _engineprivate
{
	class _ObjectInheritanceHelper
	{
		public:
		template<class O,class T>
		static void registerParent(void *oc)
		{
			O::_registerParent<O,T>(oc);
		}
		template<class T>
		static void add(Object *o)
		{
			T::_addInstance(o);
		}
		template<class T>
		static void remove(Object *o)
		{
			T::_removeInstance(o);
		}
	};
	
	class _ObjectChild
	{
		public:
		virtual void add(Object *o)
		{
		}
		virtual void remove(Object *o)
		{
		}
	};

	template<class O,class T>
	class _ObjectChildDefinition:public _ObjectChild
	{
		public:
		_ObjectChildDefinition()
		{
			_ObjectInheritanceHelper::registerParent<O,T>(this);
		}
		virtual void add(Object *o)
		{
			_ObjectInheritanceHelper::add<T>(o);
		}
		virtual void remove(Object *o)
		{
			_ObjectInheritanceHelper::remove<T>(o);
		}
		~_ObjectChildDefinition()
		{
			//TODO unregister
		}
	};
}

#define INHERIT(CHILD,PARENT) \
_engineprivate::_ObjectChildDefinition<CHILD,PARENT> _inheritance_ ## CHILD ## _ ## PARENT;


#define OBJECT \
private:\
friend class _engineprivate::EngineLayer;\
friend class _engineprivate::_ObjectChild;\
friend class _engineprivate::_ObjectInheritanceHelper;\
static std::vector<_engineprivate::_ObjectChild*>* _parent()\
{\
	static std::vector<_engineprivate::_ObjectChild*> i;\
	return &i;\
}\
template<class C,class P>\
static void _registerParent(void *ref)\
{\
	_parent()->push_back(static_cast<_engineprivate::_ObjectChildDefinition<C,P>*>(ref));\
}\
static std::vector<Object*>* _instance()\
{\
	static std::vector<Object*> i;\
	return &i;\
}\
static unsigned int _instPos(bool f)\
{\
	static unsigned int i;\
	return (f?++i:i=0);\
}\
public:\
static void _addInstance(Object *inst)\
{\
	for (std::vector<_engineprivate::_ObjectChild*>::iterator it=_parent()->begin();it!=_parent()->end();++it)\
	(*it)->add(inst);\
	_instance()->push_back(inst);\
}\
static void _removeInstance(Object *inst)\
{\
	for (std::vector<_engineprivate::_ObjectChild*>::iterator it=_parent()->begin();it!=_parent()->end();++it)\
	(*it)->remove(inst);\
	std::vector<Object*>::iterator it=std::find(_instance()->begin(),_instance()->end(),inst);\
	_instance()->erase(it);\
}\
inline static std::string *_objectName()\
{\
	static std::string _name=EngineLayer::getClassName(_OBJECT_NAME);\
	return &_name;\
}\
inline static unsigned int *_objectId()\
{\
	static unsigned int _id=_engine::obtainObjId();\
	return &_id;\
}\
public:\
virtual inline std::string objectName()\
{\
	return *_objectName();\
}\
inline static unsigned int id()\
{\
	return *_objectId();\
}\
virtual inline unsigned int objectId()\
{\
	return *_objectId();\
}\
static Object* nextInstance()\
{\
	unsigned int _pos=_instPos(1);\
	if (_pos>=_instance()->size())\
	return NULL;\
	if (_engine::isObjectDestroyed((*(_instance()))[_pos]))\
	return nextInstance();\
	return (*(_instance()))[_pos];\
}\
static Object* instance()\
{\
	if (_instance()->empty())\
	return NULL;\
	if (_engine::isObjectDestroyed((*_instance())[_instPos(0)]))\
	return nextInstance();\
	else\
	return (*_instance())[_instPos(0)];\
}\
void* operator new(size_t size)\
{\
	void *p=malloc(size);\
	if (!p)\
	throw std::bad_alloc();\
	_addInstance(static_cast<Object*>(p));\
	return p;\
}\
void operator delete(void *p)\
{\
	_removeInstance(static_cast<Object*>(p));\
	std::free(p);\
}\
private:

namespace _engineprivate
{
	class EngineLayer;
	class DepthItem;
}

namespace _ENGINESPACE
{
	using namespace _engineprivate;
	
	class Scene;

	class Object:public LinkedList<Object>
	{
		friend class _engineprivate::EngineLayer;
		friend class _engineprivate::DepthItem;
		
		private:
		std::string _objname;
		unsigned int _objid;
		//virtual void _addInstance(Object *inst);
		//virtual void _removeInstance(Object *inst
		Scene *_scene;
		bool _tagRunValue, _tagDrawValue;
		bool _destroyed;
		bool _deathMark;
		inline bool _getDestroyed()
		{
			return _destroyed;
		}
		bool _persistent;
		bool _visible;
		double _qdepth,_depth;
		std::vector<Tag*> _tags;
		std::vector<Hitbox*> _hitboxes;
		void _refreshTagRuns(bool value);
		void _refreshTagDraws(bool value);
		inline void _update();
		DepthItem *_depthItem;
		void _performDepthMove();
		inline Object* _copyDepth()
		{
			_depth=_qdepth;
			return this;
		}
		double _x,_y;
		
		public:
		inline virtual Object* newInstance()
		{
			return NULL;
		}
		virtual inline std::string objectName()
		{
			return "";
		}
		virtual inline unsigned int objectId()
		{
			return -1;
		}
		const double &x;
		const double &y;
		Object();

		virtual void onDestroy();
		inline bool isPersistent()
		{
			return _persistent;
		}
		inline bool isVisible()
		{
			return _visible;
		}
		inline bool isRunEnabled()
		{
			return _tagRunValue;
		}
		inline bool isDrawEnabled()
		{
			return _tagDrawValue;
		}
		inline Object* setVisible(bool v=1)
		{
			_visible=v;
			return this;
		}
		inline Object* unsetVisible()
		{
			_visible=0;
			return this;
		}
		Object* setPersistent(bool p=1);
		Object* unsetPersistent();
		Object* addTag(Tag *tag);
		Object* removeTag(Tag *tag);
		template<class T>
		T* at(T* t,double x,double y)//enable compability with enginenamespace
		{
			t->at(x,y);
			return t;
		}
		Object* at(double xp,double yp);
		inline Object* setPosition(double xp,double yp)
		{
			return at(xp,yp);
		}
		inline Object* move(double xp,double yp)
		{
			return at(x+xp,y+yp);
		}
		inline double getX()
		{
			return x;
		}
		inline double getY()
		{
			return y;
		}
		inline double getDepth()
		{
			return _qdepth;
		}
		inline double depth()
		{
			return getDepth();
		}
		double depth(double d);
		inline double setDepth(int d)
		{
			return depth(d);
		}
		inline void destroy()
		{
			bool d=_destroyed;
			_destroyed=1;
			if (!d)
			onDestroy();
		}
		int addCollisionBox(double px,double py,double w,double h);
		int addCollisionCicle(double px,double py,double r);
		int addCollisionLine(double px,double py,double dx,double dy);
		int addCollisionPolygon(double px,double py,double px2,double py2,double px3,double py3);
		bool getCollisions(std::vector<Object*> *vec);
		virtual void run();
		virtual void draw();
		virtual void runEnabled();
		virtual void runDisabled();
		virtual void drawEnabled();
		virtual void drawDisabled();
		virtual ~Object();
	};
}
