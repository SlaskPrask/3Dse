#include "../include/loader.h"
#include "../include/enginelayer.h"

using namespace _engineprivate;

Loader::Loader()
{
	quit=0;
	items=0;
	loaded=0;
	done=1;
	thread=NULL;
}

void Loader::add(ResourceSet *set)
{
	lock();
	items+=set->getItems();
	sets.push_back(set);
	unlock();
}

void Loader::clear()
{
	lock();
	sets.clear();
	items=0;
	loaded=0;
	unlock();
}

bool Loader::load()
{
	lock();
	done=0;
	loaded=0;
	unlock();
	if (thread)
	{
		thread->join();
		delete thread;
		thread=NULL;
	}
	thread=new std::thread(_engineprivate::loaderThread,this);
	return (done||thread->joinable());
}

void Loader::forceLoad()
{
	for (std::vector<ResourceSet*>::iterator it=sets.begin();it!=sets.end();++it)
	(*it)->load();
	done=1;
}

void Loader::exit()
{
	quit=1;
	lock();
	if (thread)
	{
		thread->join();
		delete thread;
		thread=NULL;
	}
	unlock();
}

Loader::~Loader()
{
	lock();
	if (thread)
	{
		thread->join();
		delete thread;
		thread=NULL;
	}
	unlock();
}

void Loader::threadedLoad()
{
	for (std::vector<ResourceSet*>::iterator it=sets.begin();it!=sets.end();++it)
	{
		(*it)->load(1,&loaded,&quit,this);
	}
	done=1;
}

void _engineprivate::loaderThread(Loader *loader)
{
	loader->lock();
	loader->threadedLoad();
	loader->unlock();
}
