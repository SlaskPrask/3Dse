#pragma once

#include "enginesettings.h"
#include <algorithm>
#include <vector>

namespace _engineprivate
{
	class EngineLayer;
}


namespace _ENGINESPACE
{
	using namespace _engineprivate;
	
	class Object;

	class Tag
	{
		friend class _engineprivate::EngineLayer;
		
		private:
		bool run;
		bool draw;
		std::vector<Object*> _tagObjs;
		bool detachObj(Object* o);
		bool attachObj(Object* o);
		
		public:
		Tag();
		inline bool isRunning()
		{
			return run;
		}
		inline bool isDrawing()
		{
			return draw;
		}
		Tag* enableRun();
		Tag* disableRun();
		Tag* enableDraw();
		Tag* disableDraw();
		Tag* enable();
		Tag* disable();
		unsigned int size();
		Object* get(unsigned int i);
		virtual ~Tag();
	};
}
