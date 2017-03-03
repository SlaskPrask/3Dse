#include "../include/scene.h"
#include "../include/enginelayer.h"

using namespace _engineprivate;
using namespace _ENGINESPACE;

Scene::Scene()
{
	width=height=1;
	_deathMark=0;
	_deleted=0;
	EngineLayer::instance()->switchScene(this);
}

void Scene::_deleteObjects()
{
	for(std::vector<Object*>::iterator it=_objects.begin();it!=_objects.end();++it)
	{
		EngineLayer::instance()->forcePersistent(*it);
		(*it)->destroy();
	}
}

Scene::~Scene()
{
	if (!_deathMark)
	Log::notify("Engine",std::string("Scene ")+sceneName()+"("+to_string(this)+") deleted, do not use delete on scenes just create a new instance");
	
	_deleted=1;
	
	_deleteObjects();
}

void Scene::run()
{
}
