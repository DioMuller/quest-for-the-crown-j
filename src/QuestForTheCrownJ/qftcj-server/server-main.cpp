#pragma comment(lib, "Ws2_32.lib")

#include <WinSock2.h>
#include <iostream>
#include <chrono>
#include <thread>
#include "Log.h"
#include "Server.h"
#include "Exceptions.h"
using namespace qfcserver;
using namespace qfcbase;

int main(int argc, char* argv[])
{
	Log::Debug("Initializing game server");

	try
	{
		Server server;
		while (true)
		{
			try
			{
				server.Run(12345);
			}
			catch (const SockBindError& error)
			{
				Log::Debug("Error while opening port. Waiting 3 seconds\n");
				std::this_thread::sleep_for(std::chrono::seconds(3));
			}
		}
	}
	catch (const SockError& error)
	{
		Log::Error(error.what());
	}
	catch (...)
	{
		Log::Error("Fatal error");
	}

	return 0;
}