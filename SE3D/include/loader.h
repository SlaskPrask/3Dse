#pragma once

#include "enginesettings.h"
#include "resourceset.h"
#include <thread>
#include <vector>
#include <mutex>

namespace _engineprivate
{
	using namespace _engineprivate;
	
	class Loader
	{
		private:
		std::mutex mutex;
		std::thread *thread;
		volatile bool quit;
		volatile unsigned int items;
		volatile unsigned int loaded;
		volatile bool done;
		std::vector<ResourceSet*> sets;

		public:
		Loader();
		~Loader();
		void add(ResourceSet *set);
		void clear();
		bool load();
		void forceLoad();
		void exit();
		inline void lock()
		{
			mutex.lock();
		}
		inline void unlock()
		{
			mutex.unlock();
		}
		void threadedLoad();
		inline unsigned int getItems()
		{
			return items;
		}
		inline unsigned int getLoaded()
		{
			return loaded;
		}
		inline bool getDone()
		{
			return done;
		}
		inline void leakLock()
		{
			unlock();
			lock();
		}
	};

	void loaderThread(Loader *loader);
}

