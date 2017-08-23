#pragma once
#include "enginesettings.h"
#include "platform.h"
#ifndef ANDROID
#include <SFML\System.hpp>
#endif
#include <iostream>
#include <sstream>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <vector>

namespace _engineprivate
{
	class EngineLayer;
}

namespace _engineprivate
{
	class Log
	{
		private:
		void writeLog(const std::string &logger,const std::string &str,int type);
		void forcePush(const std::string &s);
		Log();
		std::ofstream file;
		static std::string log_file;
		static clock_t igt;
		static std::vector<std::string> buffer;
		bool print;
		static bool enabled;
		bool fileoutput;

		public:
		static void enable();
		inline static Log* instance()
		{
			static Log loghandler;
			return &loghandler;
		}
		virtual ~Log();

		static void enablePrint();
		static void disablePrint();
		inline static bool isPrinting()
		{
			return instance()->enabled;
		}
		static void setFile(const std::string &str);
		static void log(int i);
		static void log(double i);
		inline static void log(const std::string &str)
		{
			instance()->writeLog("",str,Note);
		}
		inline static void error(const std::string &str)
		{
			instance()->writeLog("",str,Error);
		}
		inline static void error(const std::string &logger,const std::string &str)
		{
			instance()->writeLog(logger,str,Error);
		}
		inline static void log(const std::string &logger,const std::string &str)
		{
			instance()->writeLog(logger,str,Note);
		}
		inline static void notify(const std::string &str)
		{
			instance()->writeLog("",str,Notify);
		}
		inline static void notify(const std::string &logger,const std::string &str)
		{
			instance()->writeLog(logger,str,Notify);
		}

		enum LogType{Note,Notify,Error};
	};
}

