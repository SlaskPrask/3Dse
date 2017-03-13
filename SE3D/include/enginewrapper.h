#pragma once

#include "enginesettings.h"
	#include <string>
#include <sstream>

#include "platformgraphics.h"

namespace _ENGINESPACE
{
	class Object;
}

namespace _engineprivate
{
	template <typename T>
	std::string to_string(const T &value)
	{
		std::ostringstream os;
		os<<value;
		return os.str();
	}
}


namespace _engine
{
	using namespace _ENGINESPACE;
	
	inline unsigned int obtainObjId()
	{
		static unsigned int id=1;
		return id++;
	}
	inline unsigned int obtainScnId()
	{
		static unsigned int id=1;
		return id++;
	}
	bool isObjectDestroyed(Object* o);
	void generateTexture(GLuint *destination,int texwidth,int texheight,GLubyte *data,GLenum type);

	struct MainFunction
	{
		inline static int(*mainFunction(int(*f)(int,char**)=NULL))(int,char**)
		{
			static int(*mainf)(int,char**)=NULL;
			if (f!=NULL)
				mainf=f;
			return mainf;
		}
		MainFunction(int(*f)(int,char**))
		{
			mainFunction(f);
		}
	};

	struct ObjectListing
	{
		inline static std::vector<std::string> *listing(std::string *name)
		{
			static std::vector<std::string> names;
			if (name)
				names.push_back(*name);
			return &names;
		}
		ObjectListing(std::string *name)
		{
			listing(name);
		}
	};
	struct SceneListing
	{
		inline static std::vector<std::string> *listing(std::string *name)
		{
			static std::vector<std::string> names;
			if (name)
				names.push_back(*name);
			return &names;
		}
		SceneListing(std::string *name)
		{
			listing(name);
		}
	};

}
