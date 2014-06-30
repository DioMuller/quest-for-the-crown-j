#pragma comment(lib, "Ws2_32.lib")

#include <WinSock2.h>
#include <iostream>
#include <chrono>
#include <thread>
#include "Server.h"
#include "Exceptions.h"
using namespace qfcserver;

int main(int argc, char* argv[])
{
	std::cout << "Initializing game server" << std::endl;

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
				std::cout << "Error while opening port. Waiting 3 seconds" << std::endl;
				std::this_thread::sleep_for(std::chrono::seconds(3));
			}
		}
	}
	catch (const SockError& error)
	{
		std::cout << error.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Fatal error" << std::endl;
	}

	return 0;
}