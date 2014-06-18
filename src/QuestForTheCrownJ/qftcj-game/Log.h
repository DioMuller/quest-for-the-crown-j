#pragma once

#include <string>

namespace qfcbase
{
	////////////////////////////////////////
	// LOG DEFINITIONS
	////////////////////////////////////////
	#define LOG_ALL 4
	#define LOG_HIGH 3
	#define LOG_NORMAL 2
	#define LOG_LOW 1
	#define LOG_NONE 0

	class Log
	{
		////////////////////////////////////////
		// Attributes
		////////////////////////////////////////
		private:
			static int verbose_level;

		////////////////////////////////////////
		// Methods
		////////////////////////////////////////
		public:
			static void SetVerboseLevel(int level);

			static void Message(std::string message);
			static void Debug(std::string message);
			static void Warning(std::string message);
			static void Error(std::string message);
		private:
			static void Add(int level, std::string message);
	};
}