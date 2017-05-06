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
	LOADERLOG(Log::log("Loader","Created"));
}

void Loader::add(ResourceSet *set)
{
	lock();
	LOADERLOG(Log::log("Loader",std::string("1-Adding set ")+to_string(set)));
	items+=set->getItems();
	LOADERLOG(Log::log("Loader",std::string("1-Items now: ")+to_string(items)));
	sets.push_back(set);
	LOADERLOG(Log::log("Loader",std::string("1-Added set ")+to_string(set)));
	unlock();
}

void Loader::clear()
{
	lock();
	LOADERLOG(Log::log("Loader",std::string("2-Clearing")));
	sets.clear();
	items=0;
	loaded=0;
	LOADERLOG(Log::log("Loader",std::string("2-Cleared")));
	unlock();
}

bool Loader::load()
{
	lock();
	LOADERLOG(Log::log("Loader",std::string("3-Prepare to load")));
	done=0;
	loaded=0;
	LOADERLOG(Log::log("Loader",std::string("3-Prepared")));
	unlock();
	if (thread)
	{
		LOADERLOG(Log::log("Loader",std::string("Killing existing thred")));
		thread->join();
		delete thread;
		thread=NULL;
		LOADERLOG(Log::log("Loader",std::string("Killed existing thread")));
	}
	LOADERLOG(Log::log("Loader",std::string("Spawning new thread")));
	thread=new std::thread(_engineprivate::loaderThread,this);
	LOADERLOG(Log::log("Loader",std::string("Spawned new thread")));
	bool retval=done||thread->joinable();
	LOADERLOG(Log::log("Loader",std::string("Returning from load with ")+to_string(retval)));
	return retval;
}

void Loader::forceLoad()
{
	LOADERLOG(Log::log("Loader",std::string("Loading forced")));
	for (std::vector<ResourceSet*>::iterator it=sets.begin();it!=sets.end();++it)
	(*it)->load();
	done=1;
	LOADERLOG(Log::log("Loader",std::string("End forced load")));
}

void Loader::exit()
{
	LOADERLOG(Log::log("Loader",std::string("Exit forced")));
	quit=1;
	lock();
	LOADERLOG(Log::log("Loader",std::string("4-Exiting")));
	if (thread)
	{
		LOADERLOG(Log::log("Loader",std::string("4-Killing thread")));
		thread->join();
		LOADERLOG(Log::log("Loader",std::string("4-Thread ended")));
		delete thread;
		LOADERLOG(Log::log("Loader",std::string("4-Thread deleted")));
		thread=NULL;
	}
	LOADERLOG(Log::log("Loader",std::string("4-Exited")));
	unlock();
}

Loader::~Loader()
{
	lock();
	LOADERLOG(Log::log("Loader",std::string("5-Destroying")));
	if (thread)
	{
		LOADERLOG(Log::log("Loader",std::string("5-Killing thread")));
		thread->join();
		LOADERLOG(Log::log("Loader",std::string("5-Thread ended")));
		delete thread;
		LOADERLOG(Log::log("Loader",std::string("5-Thread deleted")));
		thread=NULL;
	}
	LOADERLOG(Log::log("Loader",std::string("5-Destroyed")));
	unlock();
}

void Loader::threadedLoad()
{
	LOADERLOG(Log::log("Loader",std::string("6-Loading")));
	for (std::vector<ResourceSet*>::iterator it=sets.begin();it!=sets.end();++it)
	{
		LOADERLOG(Log::log("Loader",std::string("6-Loading set ")+to_string(*it)));
		(*it)->load(1,&loaded,&quit,this);
	}
	done=1;
	LOADERLOG(Log::log("Loader",std::string("6-Loading done")));
}

void _engineprivate::loaderThread(Loader *loader)
{
	LOADERLOG(Log::log("Loader",std::string("Loader thread running")));
	loader->lock();
	loader->threadedLoad();
	loader->unlock();
	LOADERLOG(Log::log("Loader",std::string("Loader thread sleeping")));
}
