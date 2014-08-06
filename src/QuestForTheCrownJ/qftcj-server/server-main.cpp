#pragma comment(lib, "Ws2_32.lib")

#include <WinSock2.h>
#include <iostream>
#include <chrono>
#include <thread>

#include "Log.h"
#include "Server.h"

using namespace qfcserver;
using namespace qfcbase;

int main(int argc, char* argv[])
{
	Log::SetVerboseLevel(LOG_ALL);
	Log::Debug("Initializing game server");

	// Disables SFML errors messages, since we don't want all the "Could not load image" errors.
	std::cerr.rdbuf(0);
	sf::err().rdbuf(0);

	while (true)
	{
		try
		{
			auto server = std::make_shared<Server>(12345);
			server->Run();
			break;
		}
		catch (const std::exception& error)
		{
			Log::Error(error.what());
			Log::Debug("Waiting 3 seconds");
			std::this_thread::sleep_for(std::chrono::seconds(3));
		}
		catch (...)
		{
			Log::Error("Fatal error");
			return 1;
		}
	}

	Log::Debug("Server exited");
	return 0;
}