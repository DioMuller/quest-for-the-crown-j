#include "Log.h"
#include <iostream>
#include <time.h>

using namespace qfcbase;

////////////////////////////////////////
// Attributes
////////////////////////////////////////
int Log::verbose_level = LOG_NORMAL;

////////////////////////////////////////
// Methods
////////////////////////////////////////
void Log::SetVerboseLevel(int level)
{
	verbose_level = level;
}

void Log::Message(std::string message)
{
	Add(LOG_ALL, message);
}

void Log::Debug(std::string message)
{
	Add(LOG_HIGH, message);
}

void Log::Warning(std::string message)
{
	Add(LOG_NORMAL, message);
}

void Log::Error(std::string message)
{
	Add(LOG_LOW, message);
}

void Log::Add(int level, std::string message)
{
	if (level <= verbose_level)
	{
		time_t rawtime;
		struct tm * timeinfo;

		time(&rawtime);
		timeinfo = localtime(&rawtime);
		char* chartime = new char[32];
		asctime_s(chartime, 32, timeinfo);
		int size = strlen(chartime);
		chartime[size - 1] = '\0'; // Remove \n

		std::cout << "[" << chartime << "]: " << message << std::endl;
	}
}
