#include "../include/resourceset.h"
#include "../include/enginelayer.h"
#include "../include/loader.h"

using namespace _engineprivate;
using namespace _ENGINESPACE;

ResourceSet::ResourceSet()
{
	loads=0;
	engine_id=-1;
	_deathMark=0;
	EngineLayer::instance()->createResourceSet(this);
}

ResourceSet::~ResourceSet()
{
	if (!_deathMark)
	{
		Log::notify("Engine",std::string("Resource set ")+to_string(engine_id)+"("+to_string(this)+") deleted, do not delete resource sets yourself");
		EngineLayer::instance()->deleteExceptionResourceSet(this);
	}
	
	_unload(1);
	for(std::vector<Resource*>::iterator it=sprites.begin();it!=sprites.end();++it)
	delete (*it);
	for(std::vector<Resource*>::iterator it=sounds.begin();it!=sounds.end();++it)
	delete (*it);
	for(std::vector<Resource*>::iterator it=fonts.begin();it!=fonts.end();++it)
	delete (*it);
}

Resource* ResourceSet::addSprite(const std::string &s,bool smoothed)
{
	Resource *r=new Resource();
	r->setSprite(s,smoothed);
	sprites.push_back(r);
	return r;
}

Resource* ResourceSet::addSound(const std::string &s,bool stream)
{
	Resource *r=new Resource();
	r->setSound(s,stream);
	sounds.push_back(r);
	return r;
}

Resource* ResourceSet::addMusic(const std::string &s)
{
	return addSound(s,1);
}

Resource* ResourceSet::addFont(const std::string &s,int sz,unsigned int characters)
{
	Resource *r=new Resource();
	r->setFont(s,sz,characters);
	fonts.push_back(r);
	return r;
}

void ResourceSet::load(volatile bool threaded,volatile unsigned int *counter,volatile bool *quitter,Loader *loader)
{
	if (loads==0)
	{
		for(std::vector<Resource*>::iterator it=sprites.begin();it!=sprites.end();++it)
		{
			if (quitter&&*quitter)
			break;
			(*it)->load(threaded);
			if (counter)
			*counter+=1;
			if (loader)
				loader->leakLock();
		}
		for(std::vector<Resource*>::iterator it=sounds.begin();it!=sounds.end();++it)
		{
			if (quitter&&*quitter)
			break;
			(*it)->load(threaded);
			if (counter)
			*counter+=1;
			if (loader)
				loader->leakLock();
		}
		for(std::vector<Resource*>::iterator it=fonts.begin();it!=fonts.end();++it)
		{
			if (quitter&&*quitter)
			break;
			(*it)->load(threaded);
			if (counter)
			*counter+=1;
			if (loader)
			loader->leakLock();
		}
	}
	else
	if (counter)
	*counter+=(unsigned int)(fonts.size()+sprites.size()+sounds.size());
	
	loads++;
}

void ResourceSet::unload()
{
	EngineLayer::instance()->unloadResourceSet(this);
}

void ResourceSet::_unload(bool forced)
{
	if (loads)
	{
		if (forced)
		loads=0;
		else
		loads--;
		
		if (loads==0)
		{
			for(std::vector<Resource*>::iterator it=sprites.begin();it!=sprites.end();++it)
			(*it)->unload();
			for(std::vector<Resource*>::iterator it=sounds.begin();it!=sounds.end();++it)
			(*it)->unload();
			for(std::vector<Resource*>::iterator it=fonts.begin();it!=fonts.end();++it)
			(*it)->unload();
		}
	}
	else
	if (!forced)
	Log::notify("Engine","Attempting to unload an already unloaded set "+to_string(engine_id)+"("+to_string(this)+")");
}
