#include "../include/log.h"
#include "../include/enginelayer.h"

using namespace _engineprivate;

std::string Log::log_file="log.txt";

clock_t Log::igt=clock();
std::vector<std::string> Log::buffer;

bool Log::enabled=0;

Log::Log()
{
	print=1;
	file.open(Log::log_file.c_str());
	if (file.is_open())
	fileoutput=1;
	else
	fileoutput=0;
	#ifndef ANDROID
	enabled=1;
	#endif
}

void Log::enable()
{
	if (!enabled)
	{
		enabled=1;
		for (std::vector<std::string>::iterator it=buffer.begin();it!=buffer.end();++it)
		instance()->forcePush(*it);
	}
}

void Log::disablePrint()
{
	instance()->print=0;
}

void Log::enablePrint()
{
	instance()->print=1;
}

void Log::setFile(const std::string &str)
{
	Log::log_file=str;
}

void Log::writeLog(const std::string &logger,const std::string &str,int type)
{
	char sep=' ';
	switch (type)
	{
		default:
		case Note:
			sep=' ';
			break;
		case Notify:
			sep='-';
			break;
		case Error:
			sep='*';
			break;
	}

	std::ostringstream output;

	time_t now;
	struct tm timedata;
	time(&now);
	#ifdef UNIX
	localtime_r(&now,&timedata);
	#else
	localtime_s(&timedata,&now);
	#endif

	clock_t elapsedTime=clock()-igt;

	std::ostringstream timestr;
	timestr.fill('0');
	timestr.width(2);
	timestr << timedata.tm_mon+1 << '/';
	timestr.fill('0');
	timestr.width(2);
	timestr << timedata.tm_mday << '/' << timedata.tm_year+1900 << sep;
	timestr.fill('0');
	timestr.width(2);
	timestr << timedata.tm_hour << ':';
	timestr.fill('0');
	timestr.width(2);
	timestr << timedata.tm_min << ':';
	timestr.fill('0');
	timestr.width(2);
	timestr << timedata.tm_sec;

	std::ostringstream outbuf;
	outbuf.fill('0');
	outbuf.width(10);
	outbuf << (unsigned long long int)(((double)elapsedTime)/(double)(CLOCKS_PER_SEC)*1000.0f);
	output << timestr.str() << sep << outbuf.str() << sep;
	if (!logger.empty())
	output << '[' << logger << "] ";

	switch (type)
	{
		default:
		case Note:
		case Notify:
			output << str;
			break;
		case Error:
			output << "ERROR: " << str;
			break;
	}
	output << '\n';

	if (!enabled)
	buffer.push_back(output.str());
	else
	forcePush(output.str());
}

void Log::forcePush(const std::string &s)
{
	if (print)
	CallbackPrintLog(s);
	if (fileoutput)
	file << s;
}

void Log::log(double i)
{
	std::ostringstream str;
	str << i;
	instance()->writeLog(0,str.str().c_str(),Note);
}

void Log::log(int i)
{
	std::ostringstream str;
	str << i;
	instance()->writeLog(0,str.str().c_str(),Note);
}

Log::~Log()
{
	if (enabled)
	file.close();
}
