#pragma once

#include "enginesettings.h"
#include "enginecallback.h"

namespace _engineprivate
{
	class EngineLayer;
}

namespace _ENGINESPACE
{
	using namespace _engineprivate;
	
	class Sound
	{
		friend class _engineprivate::EngineLayer;
		
		private:
		int loadedId;
		int playId;
		bool loaded;
		int play(double l,double r,int prio,bool loop,double speed);
		void stop();
		bool looping;
		
		public:
		Sound(const std::string &file,volatile bool threaded=0);
		Sound();
		void load(const std::string &file,volatile bool threaded=0);
		void unload();
		
		~Sound();
	};
}
