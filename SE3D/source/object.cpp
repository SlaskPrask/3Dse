#include "../include/object.h"
#include "../include/enginelayer.h"
#include "../include/scene.h"

using namespace _engineprivate;
using namespace _ENGINESPACE;

Object::Object():x(_x),y(_y)
{
	_destroyed=0;
	_tagRunValue=1;
	_tagDrawValue=1;
	_qdepth=_depth=0;
	_depthItem=NULL;
	_persistent=1;
	_deathMark=0;
	_scene=NULL;
	_x=_y=0;
	_visible=1;
	EngineLayer::instance()->createObject(this);
	EngineLayer::instance()->queueDepth(this);
	//_addInstance(this);
}

Object::~Object()
{
	if (!_deathMark)
	Log::notify("Engine",std::string("Object ")+objectName()+"("+to_string(this)+") deleted, use destroy() instead");
	
	for(std::vector<Tag*>::iterator it=_tags.begin();it!=_tags.end();++it)
	EngineLayer::instance()->objClearTag(this,*it);
	EngineLayer::instance()->detachDepth(this);
	if (_scene)
	EngineLayer::instance()->untieObjectFromScene(_scene,this);
	EngineLayer::instance()->checkDeletedObject(this);

	for (std::vector<Hitbox*>::iterator it=_hitboxes.begin();it!=_hitboxes.end();++it)
	delete(*it);
	//_removeInstance(this);
}

Object* Object::at(double xp,double yp)
{
	_x=xp;
	_y=yp;
	_update();
	return this;
}

void Object::_update()
{
	for (std::vector<Hitbox*>::iterator it=_hitboxes.begin();it!=_hitboxes.end();++it)
	(*it)->move();
}

Object* Object::unsetPersistent()
{
	if (_persistent)
	{
		_scene=EngineLayer::instance()->getCurrentScene();
		if (_scene)
		{
			EngineLayer::instance()->tieObjectToScene(_scene,this);
			_persistent=0;
		}
		else
		{
			//TODO not persistence since no scene
		}
	}
	return this;
}

Object* Object::setPersistent(bool p)
{
	if (!p)
	{
		return unsetPersistent();
	}

	if (!_persistent)
	{
		EngineLayer::instance()->untieObjectFromScene(_scene,this);
		_scene=NULL;
		_persistent=1;
	}
	return this;
}

void Object::_refreshTagRuns(bool value)
{
	if (value==0)
	{
		if (_tagRunValue==1&&!_getDestroyed()&&!EngineLayer::instance()->sceneExiting(_scene))
		runDisabled();
		_tagRunValue=0;
		return;
	}
	if (_tagRunValue==0)
	{
		for(std::vector<Tag*>::iterator it=_tags.begin();it!=_tags.end();++it)
		if (!(*it)->isRunning())
		return;
		
		if (_tagRunValue==0&&!_getDestroyed()&&!EngineLayer::instance()->sceneExiting(_scene))
		runEnabled();
		_tagRunValue=1;
	}
}

void Object::_refreshTagDraws(bool value)
{
	if (value==0)
	{
		if (_tagDrawValue==1&&!_getDestroyed()&&!EngineLayer::instance()->sceneExiting(_scene))
		drawDisabled();
		_tagDrawValue=0;
		return;
	}
	if (_tagDrawValue==0)
	{
		for(std::vector<Tag*>::iterator it=_tags.begin();it!=_tags.end();++it)
		if (!(*it)->isDrawing())
		return;
		
		if (_tagDrawValue==0&&!_getDestroyed()&&!EngineLayer::instance()->sceneExiting(_scene))
		drawEnabled();
		_tagDrawValue=1;
	}
}

void Object::_performDepthMove()
{
	if (_qdepth>_depth)
	{
		_depth=_qdepth;
		_depthItem->moveUp();
	}
	else
	if (_qdepth<_depth)
	{
		_depth=_qdepth;
		_depthItem->moveDown();
	}
}

double Object::depth(double d)
{
	_qdepth=d;
	if (_depthItem)
	EngineLayer::instance()->queueDepthChange(this);
	return _qdepth;
}

Object* Object::addTag(Tag *t)
{
	EngineLayer::instance()->objAddTag(this,t);
	return this;
}

Object* Object::removeTag(Tag *t)
{
	EngineLayer::instance()->objRemoveTag(this,t);
	return this;
}

int Object::addCollisionBox(double px,double py,double w,double h)
{
	_hitboxes.push_back(EngineLayer::instance()->createCollisionBox(this,px,py,w,h));
	return _hitboxes.size()-1;
}
int Object::addCollisionCicle(double px,double py,double r)
{
	_hitboxes.push_back(EngineLayer::instance()->createCollisionCircle(this,px,py,r));
	return _hitboxes.size()-1;
}
int Object::addCollisionLine(double px,double py,double dx,double dy)
{
	_hitboxes.push_back(EngineLayer::instance()->createCollisionLine(this,px,py,dx,dy));
	return _hitboxes.size()-1;
}
int Object::addCollisionPolygon(double px,double py,double px2,double py2,double px3,double py3)
{
	_hitboxes.push_back(EngineLayer::instance()->createCollisionPolygon(this,px,py,px2,py2,px3,py3));
	return _hitboxes.size()-1;
}
bool Object::getCollisions(std::vector<Object*> *vec)
{
	vec->clear();
	for (std::vector<Hitbox*>::iterator it=_hitboxes.begin();it!=_hitboxes.end();++it)
	{
		EngineLayer::instance()->getObjectCollisions(*it,vec);
	}
	return vec->size()!=0;
}

void Object::run()
{
}
void Object::draw()
{
}
/*void Object::_addInstance(Object *inst)
{
}
void Object::_removeInstance(Object *inst)
{
}*/
void Object::runEnabled()
{
}
void Object::runDisabled()
{
}
void Object::drawEnabled()
{
}
void Object::drawDisabled()
{
}